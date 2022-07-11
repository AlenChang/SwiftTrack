package com.example.swifttrack.ui.ml;

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

public class Model {
    private static final String TAG = "MLFragment";
    private static final String MODEL_NAME = "model.ptl";
    private static final int inputNum = 300;

    private static Module mModule;

    private static List<Double> result;

    public Model(FragmentActivity activity) {
        try {
            final String FILE_NAME = assetFilePath(activity.getApplicationContext(), MODEL_NAME);
            mModule = LiteModuleLoader.load(FILE_NAME);
            Log.d("files", "Machine Learning Model path: " + FILE_NAME);
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
        final long[] inputShape = new long[]{inputNum};
        float[] inputs = new float[inputNum];
        for (int i = 0; i < inputNum; i++) {
            inputs[i] = (float) input[i];
        }

        FloatBuffer inputTensorBuffer = Tensor.allocateFloatBuffer(inputNum);
        inputTensorBuffer.put(inputs);
        Tensor inputTensor = Tensor.fromBlob(inputTensorBuffer, inputShape);

        Tensor outputTensor = mModule.forward(IValue.from(inputTensor)).toTensor();
        float[] outputs = outputTensor.getDataAsFloatArray();


        double pred = (double) outputs[0] * 100;
        Log.d(TAG, String.valueOf(pred));
        result.add(pred);

        Log.d(TAG, String.valueOf(outputs[0]));
        result.add((double) outputs[0]*100);

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
