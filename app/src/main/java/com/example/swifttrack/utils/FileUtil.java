package com.example.swifttrack.utils;

import android.util.Log;

import com.example.swifttrack.MainActivity;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;

public class FileUtil {
    private static MainActivity mainActivity;

    public static void init(MainActivity mainActivity) {
        FileUtil.mainActivity = mainActivity;
    }

    @Deprecated
    public static void readMusic(String filePath, double[][] buffer) {
        if (buffer == null) {
            return;
        }

        File file = new File(mainActivity.getExternalFilesDir(null), filePath);
        int lineCount = 0;

        try {
            FileInputStream fileInputStream = new FileInputStream(file);
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream, StandardCharsets.UTF_8);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            String line;

            while ((line = bufferedReader.readLine()) != null && lineCount < buffer.length) {
                String[] tokens = line.split(" ");

                buffer[lineCount][0] = Double.parseDouble(tokens[0]);
                buffer[lineCount][1] = Double.parseDouble(tokens[1]);

                lineCount++;
            }

            bufferedReader.close();
            inputStreamReader.close();
            fileInputStream.close();
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    @Deprecated
    public static void writeMusic(String filePath, double[][] buffer) {
        if (buffer == null) {
            return;
        }

        File file = new File(mainActivity.getExternalFilesDir(null), filePath);

        try {
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream, StandardCharsets.UTF_8);
            BufferedWriter bufferedWriter = new BufferedWriter(outputStreamWriter);

            for (double[] sample : buffer) {
                bufferedWriter.write(sample[0] + " " + sample[1] + "\n");
            }

            bufferedWriter.close();
            outputStreamWriter.close();
            fileOutputStream.close();
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    public static FileInputStream getFileInputStream(String filePath) {
        if (filePath == null) {
            return null;
        }

        File file = new File(mainActivity.getExternalFilesDir(null), filePath);

        try {
            return new FileInputStream(file);
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
            return null;
        }
    }

    public static FileOutputStream getFileOutputStream(String filePath) {
        if (filePath == null) {
            return null;
        }

        File file = new File(mainActivity.getExternalFilesDir(null), filePath);

        File dirs1 = new File(mainActivity.getExternalFilesDir(null), "player");
        File dirs2 = new File(mainActivity.getExternalFilesDir(null), "recorder");
        File dirs3 = new File(mainActivity.getExternalFilesDir(null), "processor");
        File dirs4 = new File(mainActivity.getExternalFilesDir(null), "ml");
        if(!dirs1.exists()){
            dirs1.mkdir();
        }
        if(!dirs2.exists()){
            dirs2.mkdir();
        }
        if(!dirs3.exists()){
            dirs3.mkdir();
        }
        if(!dirs4.exists()){
            dirs4.mkdir();
        }
        try {
            return new FileOutputStream(file);
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
            return null;
        }
    }

    public static int streamReadMusic(BufferedReader reader, float[] buffer) {
        if (buffer == null) {
            return 0;
        }

        int lineCount = 0;

        try {
            while (lineCount < buffer.length / 2) {
                String line = reader.readLine();
                if (line == null) {
                    break;
                }

                String[] tokens = line.split(" ");
                buffer[2 * lineCount] = (float) Double.parseDouble(tokens[0]);
                buffer[2 * lineCount + 1] = (float) Double.parseDouble(tokens[1]);

                lineCount++;
            }

            return lineCount;
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
            return 0;
        }
    }

    public static void streamWriteMusic(BufferedWriter writer, float[] buffer) {
        if (buffer == null) {
            return;
        }

        try {
            for (int i = 0; i < buffer.length; i += 2) {
                String line = buffer[i] + " " + buffer[i + 1] + "\n";
                writer.write(line);
            }
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    public static void streamWriteResult(BufferedWriter writer, double[][] result) {
        if (result == null) {
            return;
        }

        try {
            for (double[] sample : result) {
                String line = sample[0] + " " + sample[1] + " " + sample[2] + " " + sample[3] + "\n";
                writer.write(line);
            }
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    public static void streamWriteTOF(BufferedWriter writer, double[] result) {
        if (result == null) {
            return;
        }

        try {
            for (double sample : result) {
                String line = sample + "\n";
                writer.write(line);
            }
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    public static void streamWriteSingleTOF(BufferedWriter writer, double result) {
        try {
            String line = result + "\n";
            writer.write(line);
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    public static void streamWriteCIR(BufferedWriter writer, double[] result) {
        if (result == null) {
            return;
        }
        double sum = 0.0;
        for (int i = 0; i < 300; i++) {
            sum += result[i];
        }
        double dis=1e-6;
        if (Math.abs(sum - 0.0) < dis) {
            return;
        }

        try {
            StringBuilder stringBuilder = new StringBuilder();
            for (double sample : result) {
                stringBuilder.append(sample).append(",");
            }
            stringBuilder.delete(stringBuilder.length()-1, stringBuilder.length());
            stringBuilder.append("\n");
            writer.write(stringBuilder.toString());
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    @Deprecated
    public static String streamReadLine(FileInputStream stream) {
        if (stream == null) {
            return null;
        }

        StringBuilder sb = new StringBuilder();
        int i;

        try {
            while ((i = stream.read()) != -1) {
                char c = (char) i;
                if (c != '\n') {
                    sb.append(c);
                } else {
                    break;
                }
            }
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
            return null;
        }

        return sb.toString();
    }

    @Deprecated
    public static void streamWriteLine(FileOutputStream stream, String s) {
        if (stream == null) {
            return;
        }

        try {
            stream.write((s + "\n").getBytes(StandardCharsets.UTF_8));
        } catch (IOException e) {
            Log.e("FileUtil", e.getMessage());
        }
    }

    public static void deleteFile(String filePath) {
        if (filePath == null) {
            return;
        }

        File file = new File(mainActivity.getExternalFilesDir(null), filePath);

        if (file.exists() && !file.delete()) {
            Log.w("FileUtil", "Delete " + filePath + " failed.");
        }
    }
}
