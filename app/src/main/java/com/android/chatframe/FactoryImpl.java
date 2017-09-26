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

import android.content.Context;
import android.os.Process;
import android.util.SparseArray;

import com.android.chatframe.config.BugleCarrierConfigValuesLoader;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.DataModelImpl;
import com.android.chatframe.datamodel.MemoryCacheManager;
import com.android.chatframe.ui.UIIntents;
import com.android.chatframe.ui.UIIntentsImpl;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.BugleApplicationPrefs;
import com.android.chatframe.util.BugleGservices;
import com.android.chatframe.util.BugleGservicesImpl;
import com.android.chatframe.util.BuglePrefs;
import com.android.chatframe.util.BugleWidgetPrefs;
import com.android.chatframe.util.LogUtil;
import com.android.chatframe.util.MediaUtil;
import com.android.chatframe.util.BugleSubscriptionPrefs;
import com.android.chatframe.util.MediaUtilImpl;
import com.android.chatframe.util.OsUtil;

class FactoryImpl extends Factory {
    private BugleApplication mApplication;
    private DataModel mDataModel;
    private BugleGservices mBugleGservices;
    private BugleApplicationPrefs mBugleApplicationPrefs;
    private BugleWidgetPrefs mBugleWidgetPrefs;
    private Context mApplicationContext;
    private UIIntents mUIIntents;
    private MemoryCacheManager mMemoryCacheManager;
    private SparseArray<BugleSubscriptionPrefs> mSubscriptionPrefs;
    private MediaUtil mMediaUtil;
    private BugleCarrierConfigValuesLoader mCarrierConfigValuesLoader;

    private FactoryImpl() {

    }

    public static Factory register(final Context applicationContext,
            final BugleApplication application) {
        // This only gets called once (from BugleApplication.onCreate), but its not called in tests.
        Assert.isTrue(!sRegistered);
        Assert.isNull(Factory.get());

        final FactoryImpl factory = new FactoryImpl();
        Factory.setInstance(factory);
        sRegistered = true;

        // At this point Factory is published. Services can now get initialized and depend on
        // Factory.get().
        factory.mApplication = application;
        factory.mApplicationContext = applicationContext;
        factory.mMemoryCacheManager = new MemoryCacheManager();
        factory.mBugleGservices = new BugleGservicesImpl(applicationContext);
        factory.mBugleApplicationPrefs = new BugleApplicationPrefs(applicationContext);
        factory.mDataModel = new DataModelImpl(applicationContext);
        factory.mBugleWidgetPrefs = new BugleWidgetPrefs(applicationContext);
        factory.mUIIntents = new UIIntentsImpl();
        factory.mMediaUtil = new MediaUtilImpl();
        factory.mSubscriptionPrefs = new SparseArray<BugleSubscriptionPrefs>();
        factory.mCarrierConfigValuesLoader = new BugleCarrierConfigValuesLoader(applicationContext);

        Assert.initializeGservices(factory.mBugleGservices);
        LogUtil.initializeGservices(factory.mBugleGservices);

        if (OsUtil.hasRequiredPermissions()) {
            factory.onRequiredPermissionsAcquired();
        }

        return factory;
    }

    @Override
    public void onRequiredPermissionsAcquired() {
        if (sInitialized) {
            return;
        }
        sInitialized = true;

        mApplication.initializeSync(this);

        final Thread asyncInitialization = new Thread() {
            @Override
            public void run() {
                Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
                mApplication.initializeAsync(FactoryImpl.this);
            }
        };
        asyncInitialization.start();
    }

    @Override
    public Context getApplicationContext() {
        return mApplicationContext;
    }

    @Override
    public DataModel getDataModel() {
        return mDataModel;
    }

    @Override
    public BugleGservices getBugleGservices() {
        return mBugleGservices;
    }

    @Override
    public BuglePrefs getApplicationPrefs() {
        return mBugleApplicationPrefs;
    }

    @Override
    public BuglePrefs getWidgetPrefs() {
        return mBugleWidgetPrefs;
    }

    @Override
    public BuglePrefs getSubscriptionPrefs(int subId) {
        BugleSubscriptionPrefs pref = mSubscriptionPrefs.get(subId);
        if (pref == null) {
            synchronized (this) {
                if ((pref = mSubscriptionPrefs.get(subId)) == null) {
                    pref = new BugleSubscriptionPrefs(getApplicationContext(), subId);
                    mSubscriptionPrefs.put(subId, pref);
                }
            }
        }
        return pref;
    }

    @Override
    public UIIntents getUIIntents() {
        return mUIIntents;
    }

    @Override
    public MemoryCacheManager getMemoryCacheManager() {
        return mMemoryCacheManager;
    }


    @Override
    public void reclaimMemory() {
        mMemoryCacheManager.reclaimMemory();
    }

    @Override
    public void onActivityResume() {
    }

    @Override
    public MediaUtil getMediaUtil() {
        return mMediaUtil;
    }

    @Override
    public BugleCarrierConfigValuesLoader getCarrierConfigValuesLoader() {
        return mCarrierConfigValuesLoader;
    }
}
