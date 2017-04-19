package nhuisman.vtc.edu.vt_bb1;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

/**
 * A placeholder fragment containing a simple view.
 */
public class MainActivityFragment extends Fragment {

    public MainActivityFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_main, container, false);
    }

    private void ConnectionStatusClicked() {
        int x = 1;
    }

    private void ModeClicked() {
        int x = 1;
    }

    private void ForwardClicked() {
        int x = 1;
    }

    private void ReverseClicked() {
        int x = 1;
    }

    private void RightClicked() {
        int x = 1;
    }

    private void LeftClicked() {
        int x = 1;
    }
}
