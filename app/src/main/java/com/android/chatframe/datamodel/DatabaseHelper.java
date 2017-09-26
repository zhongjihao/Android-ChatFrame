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

package com.android.chatframe.datamodel;

import com.android.chatframe.BugleApplication;
import com.android.chatframe.R;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.Assert.DoesNotRunOnMainThread;
import com.android.chatframe.util.LogUtil;

import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.provider.BaseColumns;
import android.util.Log;

/**
 * TODO: Open Issues:
 * - Should we be storing the draft messages in the regular messages table or should we have a
 *   separate table for drafts to keep the normal messages query as simple as possible?
 */

/**
 * Allows access to the SQL database.  This is package private.
 */
public class DatabaseHelper extends SQLiteOpenHelper {
    private static final String TAG = "DatabaseHelper";
    public static final String DATABASE_NAME = "bugle_db";

    private static final int getDatabaseVersion(final Context context) {
        return Integer.parseInt(context.getResources().getString(R.string.database_version));
    }

    /** Table containing names of all other tables and views */
    private static final String MASTER_TABLE = "sqlite_master";
    /** Column containing the name of the tables and views */
    private static final String[] MASTER_COLUMNS = new String[] { "name", };

    // Table names
    public static final String CONVERSATIONS_TABLE = "conversations";
    public static final String MESSAGES_TABLE = "messages";
    public static final String PARTS_TABLE = "parts";

    // Conversations table schema
    public static class ConversationColumns implements BaseColumns {
        //TODO ...
    }

   
    // Messages table schema
    public static class MessageColumns implements BaseColumns {
      //TODO ...
    }

    // Parts table schema
    // A part may contain text or a media url, but not both.
    public static class PartColumns implements BaseColumns {
      //TODO ...
    }

    private static final Object sLock = new Object();
    private final Context mApplicationContext;
    private static DatabaseHelper sHelperInstance;      // Protected by sLock.

    private final Object mDatabaseWrapperLock = new Object();
    private DatabaseWrapper mDatabaseWrapper;           // Protected by mDatabaseWrapperLock.
    private final DatabaseUpgradeHelper mUpgradeHelper = new DatabaseUpgradeHelper();

    /**
     * Get a (singleton) instance of {@link DatabaseHelper}, creating one if there isn't one yet.
     * This is the only public method for getting a new instance of the class.
     * @param context Should be the application context (or something that will live for the
     * lifetime of the application).
     * @return The current (or a new) DatabaseHelper instance.
     */
    public static DatabaseHelper getInstance(final Context context) {
        synchronized (sLock) {
            if (sHelperInstance == null) {
                sHelperInstance = new DatabaseHelper(context);
            }
            return sHelperInstance;
        }
    }

    /**
     * Private constructor, used from {@link #getInstance()}.
     * @param context Should be the application context (or something that will live for the
     * lifetime of the application).
     */
    private DatabaseHelper(final Context context) {
        super(context, DATABASE_NAME, null, getDatabaseVersion(context), null);
        mApplicationContext = context;
    }

    /**
     * Test method that always instantiates a new DatabaseHelper instance. This should
     * be used ONLY by the tests and never by the real application.
     * @param context Test context.
     * @return Brand new DatabaseHelper instance.
     */
    static DatabaseHelper getNewInstanceForTest(final Context context) {
        Assert.isEngBuild();
        Assert.isTrue(BugleApplication.isRunningTests());
        return new DatabaseHelper(context);
    }

    /**
     * Get the (singleton) instance of @{link DatabaseWrapper}.
     * <p>The database is always opened as a writeable database.
     * @return The current (or a new) DatabaseWrapper instance.
     */
    @DoesNotRunOnMainThread
    DatabaseWrapper getDatabase() {
        // We prevent the main UI thread from accessing the database here since we have to allow
        // public access to this class to enable sub-packages to access data.
        Assert.isNotMainThread();

        synchronized (mDatabaseWrapperLock) {
            if (mDatabaseWrapper == null) {
                mDatabaseWrapper = new DatabaseWrapper(mApplicationContext, getWritableDatabase());
            }
            return mDatabaseWrapper;
        }
    }

    @Override
    public void onDowngrade(final SQLiteDatabase db, final int oldVersion, final int newVersion) {
        mUpgradeHelper.onDowngrade(db, oldVersion, newVersion);
    }

    /**
     * Drops and recreates all tables.
     */
    public static void rebuildTables(final SQLiteDatabase db) {
        // Drop tables first, then views, and indices.
        dropAllTables(db);
        dropAllViews(db);
        dropAllIndexes(db);
        dropAllTriggers(db);

        // Recreate the whole database.
        createDatabase(db);
    }

    /**
     * Drop and rebuild a given view.
     */
    static void rebuildView(final SQLiteDatabase db, final String viewName,
            final String createViewSql) {
        dropView(db, viewName, true /* throwOnFailure */);
        db.execSQL(createViewSql);
    }

    private static void dropView(final SQLiteDatabase db, final String viewName,
            final boolean throwOnFailure) {
        final String dropPrefix = "DROP VIEW IF EXISTS ";
        try {
            db.execSQL(dropPrefix + viewName);
        } catch (final SQLException ex) {
            if (LogUtil.isLoggable(LogUtil.BUGLE_TAG, LogUtil.DEBUG)) {
                LogUtil.d(LogUtil.BUGLE_TAG, "unable to drop view " + viewName + " "
                        + ex);
            }

            if (throwOnFailure) {
                throw ex;
            }
        }
    }

    /**
     * Drops all user-defined tables from the given database.
     */
    private static void dropAllTables(final SQLiteDatabase db) {
        final Cursor tableCursor =
                db.query(MASTER_TABLE, MASTER_COLUMNS, "type='table'", null, null, null, null);
        if (tableCursor != null) {
            try {
                final String dropPrefix = "DROP TABLE IF EXISTS ";
                while (tableCursor.moveToNext()) {
                    final String tableName = tableCursor.getString(0);

                    // Skip special tables
                    if (tableName.startsWith("android_") || tableName.startsWith("sqlite_")) {
                        continue;
                    }
                    try {
                        db.execSQL(dropPrefix + tableName);
                    } catch (final SQLException ex) {
                        if (LogUtil.isLoggable(LogUtil.BUGLE_TAG, LogUtil.DEBUG)) {
                            LogUtil.d(LogUtil.BUGLE_TAG, "unable to drop table " + tableName + " "
                                    + ex);
                        }
                    }
                }
            } finally {
                tableCursor.close();
            }
        }
    }

    /**
     * Drops all user-defined triggers from the given database.
     */
    private static void dropAllTriggers(final SQLiteDatabase db) {
        final Cursor triggerCursor =
                db.query(MASTER_TABLE, MASTER_COLUMNS, "type='trigger'", null, null, null, null);
        if (triggerCursor != null) {
            try {
                final String dropPrefix = "DROP TRIGGER IF EXISTS ";
                while (triggerCursor.moveToNext()) {
                    final String triggerName = triggerCursor.getString(0);

                    // Skip special tables
                    if (triggerName.startsWith("android_") || triggerName.startsWith("sqlite_")) {
                        continue;
                    }
                    try {
                        db.execSQL(dropPrefix + triggerName);
                    } catch (final SQLException ex) {
                        if (LogUtil.isLoggable(LogUtil.BUGLE_TAG, LogUtil.DEBUG)) {
                            LogUtil.d(LogUtil.BUGLE_TAG, "unable to drop trigger " + triggerName +
                                    " " + ex);
                        }
                    }
                }
            } finally {
                triggerCursor.close();
            }
        }
    }

    /**
     * Drops all user-defined views from the given database.
     */
    private static void dropAllViews(final SQLiteDatabase db) {
        final Cursor viewCursor =
                db.query(MASTER_TABLE, MASTER_COLUMNS, "type='view'", null, null, null, null);
        if (viewCursor != null) {
            try {
                while (viewCursor.moveToNext()) {
                    final String viewName = viewCursor.getString(0);
                    dropView(db, viewName, false /* throwOnFailure */);
                }
            } finally {
                viewCursor.close();
            }
        }
    }

    /**
     * Drops all user-defined views from the given database.
     */
    private static void dropAllIndexes(final SQLiteDatabase db) {
        final Cursor indexCursor =
                db.query(MASTER_TABLE, MASTER_COLUMNS, "type='index'", null, null, null, null);
        if (indexCursor != null) {
            try {
                final String dropPrefix = "DROP INDEX IF EXISTS ";
                while (indexCursor.moveToNext()) {
                    final String indexName = indexCursor.getString(0);
                    try {
                        db.execSQL(dropPrefix + indexName);
                    } catch (final SQLException ex) {
                        if (LogUtil.isLoggable(LogUtil.BUGLE_TAG, LogUtil.DEBUG)) {
                            LogUtil.d(LogUtil.BUGLE_TAG, "unable to drop index " + indexName + " "
                                    + ex);
                        }
                    }
                }
            } finally {
                indexCursor.close();
            }
        }
    }

    private static void createDatabase(final SQLiteDatabase db) {
        Log.d(TAG, "createDatabase");
//        for (final String sql : CREATE_TABLE_SQLS) {
//            db.execSQL(sql);
//        }
//
//        for (final String sql : CREATE_INDEX_SQLS) {
//            db.execSQL(sql);
//        }
//
//        for (final String sql : CREATE_VIEW_SQLS) {
//            db.execSQL(sql);
//        }
//
//        for (final String sql : CREATE_TRIGGER_SQLS) {
//            db.execSQL(sql);
//        }

        // Enable foreign key constraints
        //db.execSQL("PRAGMA foreign_keys=ON;");

        DataModel.get().onCreateTables(db);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        createDatabase(db);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        Log.d(TAG, "onUpgrade, db.version:" + db.getVersion() + ", oldVersion:" + oldVersion + ", newVersion:" + newVersion);
        mUpgradeHelper.doOnUpgrade(db, oldVersion, newVersion);
    }
}
