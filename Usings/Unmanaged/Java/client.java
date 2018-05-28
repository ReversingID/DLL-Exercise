/*
Client sederhana untuk menggunakan unmanaged.dll (lihat bagian Creations/Unmanaged)

Memanggil unmanaged.dll yang memiliki fungsi:
    - world (static)
    - calculate

Menggunakan JNA (Java Native Access) untuk memuat dan menjalankan kode native.
Ada beberapa alternative, lihat lebih banyak contohnya di repository `CodeInterop-Exercise`

Compile:
    $ javac -cp jna.jar client.java

Run: 
    (windows)
    $ java -cp "jna.jar;." client

    (linux)
    $ java -cp "jna.jar:." client

Pastikan JNA.jar berada dalam CLASSPATH atau dapat diakses oleh project.
*/

/*
Pada dasarnya, hanya JNI yang didukung sebagai satu-satunya cara untuk berinteraksi 
antara kode Java dan kode native.
Hal ini akan berbeda apabila proposal JEP 191 Foreign Function Interface benar-benar 
telah diimplementasikan dan dapat digunakan.

JNA merupakan library luar yang tidak disediakan secara default oleh Java (Oracle JDK 
maupun OpenJDK). JNA dapat diperoleh melalui

    https://github.com/java-native-access/jna

JNA 4.5.1 digunakan sebagai contoh.
*/

// Setidaknya ketiga modul ini harus tersedia.
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

public class client
{
    // Buat sebuah interface (alternatif-1)
    // Alternatif ini merupakan cara standard dan stabil untuk memetakan 
    // fungsi native dan kode Java.
    public interface Unmanaged extends Library 
    {
        Unmanaged instance = (Unmanaged) Native.loadLibrary("unmanaged.dll", Unmanaged.class);

        // Deklarasikan fungsi-fungsi yang dapat dipanggil (impor dari DLL)
        public void world();
        public int  calculate(int n);
    }

    // Buat sebuah kelas statis (alternatif-2)
    public static class UnmanagedC
    {
        // Deklarasikan fungsi-fungsi yang dapat dipanggil (impor dari DLL)
        public static native void world();
        public static native int  calculate(int n);

        static 
        {
            Native.register("unmanaged.dll");
        }
    }


    public static void main(String[] args)
    {
        int result;

        System.out.println("Starting...");
        
        // Memanggil fungsi static world() dengan pendekatan alternatif pertama
        Unmanaged.instance.world();

        // Memanggil calculate() dengan pendekatan alternatif kedua
        System.out.println("Hasil dari calculate(0) adalah " + UnmanagedC.calculate(0));
        
    }
}