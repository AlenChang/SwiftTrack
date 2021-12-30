package com.example.swifttrack.ui.lstm;

import android.app.Presentation;
import android.content.Context;
import android.util.Log;

import androidx.fragment.app.FragmentActivity;

import org.pytorch.IValue;
import org.pytorch.LiteModuleLoader;
import org.pytorch.Module;
import org.pytorch.Tensor;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class LSTMModel {
    private static final String TAG = "LSTMFragment";
    private static final String MODEL_NAME = "test_model.ptl";
    private static final int inputNum = 150;

    private static final float[] h0 = new float[30];
    private static final FloatBuffer h0TensorBuffer = Tensor.allocateFloatBuffer(30).put(h0);
    private static Tensor h0Tensor = Tensor.fromBlob(h0TensorBuffer, new long[]{1,1,30});

    private static final float[] c0 = new float[30];
    private static final FloatBuffer c0TensorBuffer = Tensor.allocateFloatBuffer(30).put(c0);
    private static Tensor c0Tensor = Tensor.fromBlob(c0TensorBuffer, new long[]{1,1,30});


    private static Module mModule;

    private static List<Double> result;

    public LSTMModel(FragmentActivity activity) {
        try {
            mModule = LiteModuleLoader.load(assetFilePath(activity.getApplicationContext(), MODEL_NAME));
        } catch (IOException e) {
            Log.e(TAG, "Error reading assets", e);
            activity.finish();
        }

        result = new ArrayList<>();
    }

    public static double[] getHistoryData(double[] input, int winLen) {
        prediction(input);
        double[] data = new double[winLen];
        int n = result.size();

        if (winLen > n) {
            for (int i = 0; i < n; i++) {
                data[i] = result.get(i);
            }
        } else {
            for (int i = winLen - 1, p = n - 1; i >= 0; i--, p--) {
                data[i] = result.get(p);
            }
        }

        return data;
    }

    public static void prediction(double[] input) {
        final long[] inputShape = new long[]{1, inputNum};
        float[] inputs = new float[inputNum];
        for (int i = 0; i < inputNum; i++) {
            inputs[i] = (float) input[i];
        }

        FloatBuffer inputTensorBuffer = Tensor.allocateFloatBuffer(inputNum);
        inputTensorBuffer.put(inputs);
        Tensor inputTensor = Tensor.fromBlob(inputTensorBuffer, inputShape);

        IValue[] outputTensor = mModule.forward(IValue.from(inputTensor), IValue.from(h0Tensor), IValue.from(c0Tensor)).toTuple();
        float[] outputs = outputTensor[0].toTensor().getDataAsFloatArray();
        h0Tensor = outputTensor[1].toTensor();
        c0Tensor = outputTensor[2].toTensor();

        double pred = (double) outputs[0] * 100;
        Log.d(TAG, String.valueOf(pred));
        result.add(pred);
    }

    private String assetFilePath(Context context, String assetName) throws IOException {
        File file = new File(context.getFilesDir(), assetName);
        if (file.exists() && file.length() > 0) {
            return file.getAbsolutePath();
        }

        try (InputStream is = context.getAssets().open(assetName)) {
            try (OutputStream os = new FileOutputStream(file)) {
                byte[] buffer = new byte[4 * 1024];
                int read;
                while ((read = is.read(buffer)) != -1) {
                    os.write(buffer, 0, read);
                }
                os.flush();
            }
            return file.getAbsolutePath();
        }
    }
}

