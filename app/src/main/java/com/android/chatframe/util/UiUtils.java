package com.android.chatframe.util;

import com.android.chatframe.Factory;

import android.content.Context;
import android.view.Gravity;
import android.widget.Toast;

public class UiUtils {

    /** Show a simple toast at the bottom */
    public static void showToastAtBottom(final int messageId) {
        UiUtils.showToastAtBottom(getApplicationContext().getString(messageId));
    }

    /** Show a simple toast at the bottom */
    public static void showToastAtBottom(final String message) {
        final Toast toast = Toast.makeText(getApplicationContext(), message,
                Toast.LENGTH_LONG);
        toast.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL, 0, 0);
        toast.show();
    }

    /** Show a simple toast at the default position */
    public static void showToast(final int messageId) {
        final Toast toast = Toast
                .makeText(getApplicationContext(), getApplicationContext()
                        .getString(messageId), Toast.LENGTH_LONG);
        toast.setGravity(Gravity.CENTER_HORIZONTAL, 0, 0);
        toast.show();
    }

    /** Show a simple toast at the default position */
    public static void showToast(final int pluralsMessageId, final int count) {
        final Toast toast = Toast.makeText(
                getApplicationContext(),
                getApplicationContext().getResources().getQuantityString(
                        pluralsMessageId, count), Toast.LENGTH_LONG);
        toast.setGravity(Gravity.CENTER_HORIZONTAL, 0, 0);
        toast.show();
    }

    private static Context getApplicationContext() {
        return Factory.get().getApplicationContext();
    }
}
