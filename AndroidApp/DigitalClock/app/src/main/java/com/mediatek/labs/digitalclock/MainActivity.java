package com.mediatek.labs.digitalclock;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends AppCompatActivity {

    private boolean m_state;
    private String m_ssid = "your_SSID";
    private String m_pwd = "your_pwd";
    private String m_customData = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        elianInitSmartConnection();
        m_state = false;

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!m_state) {
                    elianStartSmartConnection(m_ssid, m_pwd, m_customData);
                    m_state = true;
                    Snackbar.make(view, "Start Smart connection", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();
                }
                else
                {
                    elianStopSmartConnection();
                    m_state = false;
                    Snackbar.make(view, "Stop Smart connection", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();
                }
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    static {
        System.loadLibrary("elian-wrapper-jni");
    }
    public native void elianInitSmartConnection();
    public native void elianStartSmartConnection(String SSID, String password, String customData);
    public native void elianStopSmartConnection();
}
