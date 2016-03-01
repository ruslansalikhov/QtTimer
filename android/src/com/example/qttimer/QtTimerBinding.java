package com.example.qttimer;

import android.os.Bundle;
import android.util.Log;
import java.util.ArrayList;
import java.util.List;

/*! Java class for bind the C++ method of QtTimer
 *
 *  This class is a singleton because it's a binding of a singleton C++ object
 *  All the methods are static, and they use the private singleton instance
 */
public class QtTimerBinding {
    // Singleton instance created as soon as possibile
    private static final QtTimerBinding m_instance = new QtTimerBinding();

    private static List<IQtTimerListener> m_listeners = new ArrayList<IQtTimerListener>();

    // private constructor
    private QtTimerBinding() {
        // nothing to do here
    }

    // these function will be called from c++
    static public void emitTimeout() {
        Log.v("TEST", "emitTimer");
        for (IQtTimerListener listener : m_listeners)
            listener.onTimeout();
    }

    public static void addListener(IQtTimerListener listener) {
        Log.v("TEST", "Added");
        m_listeners.add(listener);
    }

    public static native void start(int timeout);
    public static native void stop();
    // initialize a should be called in thread where it will be used
    public static native void initialize();
    // For attaching qt event loop to host
    public static native void processEvents();

    static {
        System.loadLibrary("dl");
        System.loadLibrary("log");
        System.loadLibrary("stdc++");
        System.loadLibrary("m");
        System.loadLibrary("c");
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("Qt5Core");
        System.loadLibrary("Qt5AndroidExtras");
        System.loadLibrary("QtTimer");
    }
}
