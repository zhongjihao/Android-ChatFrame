/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.chatframe;

import android.app.Application;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Looper;
import java.io.File;
import java.lang.Thread.UncaughtExceptionHandler;
import com.squareup.leakcanary.LeakCanary;
import com.squareup.leakcanary.RefWatcher;
import com.android.chatframe.config.CarrierConfigValuesLoader;
import com.android.chatframe.config.MmsConfig;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.framework.MsgDispatcher;
import com.android.chatframe.framework.RilFactory;
import com.android.chatframe.util.BugleGservices;
import com.android.chatframe.util.BuglePrefs;
import com.android.chatframe.util.BuglePrefsKeys;
import com.android.chatframe.util.DebugUtils;
import com.android.chatframe.util.LogUtil;
import com.android.chatframe.util.OsUtil;
import com.android.chatframe.util.Trace;

/**
 * The application object
 */
public class BugleApplication extends Application implements UncaughtExceptionHandler {
    private static final String TAG = LogUtil.BUGLE_TAG;

    private static boolean sRunningTests = false;
    private UncaughtExceptionHandler sSystemUncaughtExceptionHandler;

    protected static void setTestsRunning() {
        sRunningTests = true;
    }

    /**
     * @return true if we're running unit tests.
     */
    public static boolean isRunningTests() {
        return sRunningTests;
    }

    private static BugleApplication application;
    private MsgDispatcher dispatcher;
    private RefWatcher refWatcher;

    @Override
    public void onCreate() {
        Trace.beginSection("app.onCreate");
        super.onCreate();
        refWatcher = LeakCanary.install(this);
        // Note onCreate is called in both test and real application environments
        if (!sRunningTests) {
            application = this;
            // Only create the factory if not running tests
            FactoryImpl.register(getApplicationContext(), this);

            dispatcher = new MsgDispatcher(RilFactory.makeRil(this));
        } else {
            LogUtil.e(TAG, "BugleApplication.onCreate: FactoryImpl.register skipped for test run");
        }

        sSystemUncaughtExceptionHandler = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(this);
        Trace.endSection();
    }

    public static MsgDispatcher getDispatcher(){
        return application.dispatcher;
    }

    public static RefWatcher getRefWatcher(Context context) {
        BugleApplication application = (BugleApplication) context.getApplicationContext();
        return application.refWatcher;
    }

    // Called by the "real" factory from FactoryImpl.register() (i.e. not run in tests)
    public void initializeSync(final Factory factory) {
        Trace.beginSection("app.initializeSync");
        final Context context = factory.getApplicationContext();
        final BugleGservices bugleGservices = factory.getBugleGservices();
        final BuglePrefs buglePrefs = factory.getApplicationPrefs();
        final DataModel dataModel = factory.getDataModel();
        final CarrierConfigValuesLoader carrierConfigValuesLoader =
                factory.getCarrierConfigValuesLoader();

        maybeStartProfiling();

        // Initialize MMS lib
        initMmsLib(context, bugleGservices, carrierConfigValuesLoader);
        // Fixup messages in flight if we crashed and send any pending
        dataModel.onApplicationCreated();
        // Register carrier config change receiver
        if (OsUtil.isAtLeastM()) {
            registerCarrierConfigChangeReceiver(context);
        }

        Trace.endSection();
    }

    private static void initMmsLib(final Context context, final BugleGservices bugleGservices,
            final CarrierConfigValuesLoader carrierConfigValuesLoader) {
    }
    
    private static void registerCarrierConfigChangeReceiver(final Context context) {
        context.registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                LogUtil.i(TAG, "Carrier config changed. Reloading MMS config.");
                MmsConfig.loadAsync();
            }
        }, new IntentFilter("android.telephony.action.CARRIER_CONFIG_CHANGED"));
    }

    // Called from thread started in FactoryImpl.register() (i.e. not run in tests)
    public void initializeAsync(final Factory factory) {
        // Handle shared prefs upgrade & Load MMS Configuration
        Trace.beginSection("app.initializeAsync");
        maybeHandleSharedPrefsUpgrade(factory);
        MmsConfig.load();
        Trace.endSection();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();

        if (LogUtil.isLoggable(TAG, LogUtil.DEBUG)) {
            LogUtil.d(TAG, "onLowMemory");
        }
        Factory.get().reclaimMemory();
    }

    @Override
    public void uncaughtException(final Thread thread, final Throwable ex) {
        final boolean background = getMainLooper().getThread() != thread;
        if (background) {
            LogUtil.e(TAG, "Uncaught exception in background thread " + thread, ex);

            final Handler handler = new Handler(getMainLooper());
            handler.post(new Runnable() {
                @Override
                public void run() {
                    sSystemUncaughtExceptionHandler.uncaughtException(thread, ex);
                }
            });
        } else {
            sSystemUncaughtExceptionHandler.uncaughtException(thread, ex);
        }
    }

    private void maybeStartProfiling() {
        // App startup profiling support. To use it:
        //  adb shell setprop log.tag.BugleProfile DEBUG
        //  #   Start the app, wait for a 30s, download trace file:
        //  adb pull /data/data/com.android.messaging/cache/startup.trace /tmp
        //  # Open trace file (using adt/tools/traceview)
        if (android.util.Log.isLoggable(LogUtil.PROFILE_TAG, android.util.Log.DEBUG)) {
            // Start method tracing with a big enough buffer and let it run for 30s.
            // Note we use a logging tag as we don't want to wait for gservices to start up.
            final File file = DebugUtils.getDebugFile("startup.trace", true);
            if (file != null) {
                android.os.Debug.startMethodTracing(file.getAbsolutePath(), 160 * 1024 * 1024);
                new Handler(Looper.getMainLooper()).postDelayed(
                       new Runnable() {
                            @Override
                            public void run() {
                                android.os.Debug.stopMethodTracing();
                                // Allow world to see trace file
                                DebugUtils.ensureReadable(file);
                                LogUtil.d(LogUtil.PROFILE_TAG, "Tracing complete - "
                                     + file.getAbsolutePath());
                            }
                        }, 30000);
            }
        }
    }

    private void maybeHandleSharedPrefsUpgrade(final Factory factory) {
        final int existingVersion = factory.getApplicationPrefs().getInt(
                BuglePrefsKeys.SHARED_PREFERENCES_VERSION,
                BuglePrefsKeys.SHARED_PREFERENCES_VERSION_DEFAULT);
        final int targetVersion = Integer.parseInt(getString(R.string.pref_version));
        if (targetVersion > existingVersion) {
            LogUtil.i(LogUtil.BUGLE_TAG, "Upgrading shared prefs from " + existingVersion +
                    " to " + targetVersion);
            try {
                // Perform upgrade on application-wide prefs.
                factory.getApplicationPrefs().onUpgrade(existingVersion, targetVersion);
                factory.getSubscriptionPrefs(1).onUpgrade(existingVersion, targetVersion);
                factory.getApplicationPrefs().putInt(BuglePrefsKeys.SHARED_PREFERENCES_VERSION,
                        targetVersion);
            } catch (final Exception ex) {
                // Upgrade failed. Don't crash the app because we can always fall back to the
                // default settings.
                LogUtil.e(LogUtil.BUGLE_TAG, "Failed to upgrade shared prefs", ex);
            }
        } else if (targetVersion < existingVersion) {
            // We don't care about downgrade since real user shouldn't encounter this, so log it
            // and ignore any prefs migration.
            LogUtil.e(LogUtil.BUGLE_TAG, "Shared prefs downgrade requested and ignored. " +
                    "oldVersion = " + existingVersion + ", newVersion = " + targetVersion);
        }
    }
}
