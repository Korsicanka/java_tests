import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.lang.Exception;
import java.io.FileNotFoundException;
import java.io.IOException;


class LittleEndianDataInputStream extends DataInputStream {
	LittleEndianDataInputStream(InputStream is) {
		super(is);
	}
}

class LittleEndianDataOutputStream extends DataOutputStream {
	LittleEndianDataOutputStream(OutputStream os) {
		super(os);
	}
}

public class test {
	public static String IFileName = "input.txt";
	public static String OFileName = "output.txt";
	public static String leOFileName = "le_output.txt";

	public static void main(String[] str) {
		System.out.println("Test of new le/be stream");

		DataInputStream  dis;
		DataOutputStream dos;
		LittleEndianDataInputStream ledis;
		LittleEndianDataOutputStream ledos;

		try {

			dis = new DataInputStream(new FileInputStream(IFileName));
			dos = new DataOutputStream(new FileOutputStream(OFileName));
			ledis = new LittleEndianDataInputStream(new FileInputStream(IFileName));
			ledos = new LittleEndianDataOutputStream(new FileOutputStream(leOFileName));

		} catch (FileNotFoundException e) {
			System.out.println("Files were not found! Terminating.");
			return;
		}


		try {
			dos.writeInt(dis.readInt());
			ledos.writeInt(ledis.readInt());
		} catch (IOException e) {
			System.err.println("Cannot read/write streams!");
		}

		try {

			dis.close();
			dos.close();
			ledis.close();
			ledos.close();

		} catch (IOException e) {
			if (dis == null) {
				System.err.println("Cannot close DataOutputStream!");
			} else
			if (dos == null) {
				System.err.println("Some error occured while closing streams: " + e);
				e.printStackTrace();
			} else {
				System.err.println("Cannot close DataInputStream!");
			}
		}
	}
}
