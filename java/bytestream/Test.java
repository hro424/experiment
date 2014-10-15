import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;


public class Test {
    private ByteArrayOutputStream body;
    private ByteArrayOutputStream symbolTable;
    private int cur;

    Test() {
        cur = 0;
        body = new ByteArrayOutputStream();
        symbolTable = new ByteArrayOutputStream();
    }

    private void dump(byte[] buf) {
        for (byte b : buf) {
            System.out.printf("0x%x ", b);
        }
    }

    void dump() {
        dump(body.toByteArray());
        System.out.println();
        dump(symbolTable.toByteArray());
        System.out.println();
    }

    void setup(boolean cont, boolean inter, int alt) throws IOException {
        byte[] buf = ByteBuffer.allocate(4).putInt(alt).array();
        buf[0] |= (cont ? 0x80 : 0) | (inter ? 0x40 : 0);
        body.write(buf);
    }

    void write(String s) throws IOException {
        int len = s.length();
        body.write(ByteBuffer.allocate(4).putInt(cur).array());
        body.write(ByteBuffer.allocate(4).putInt(len).array());
        this.cur += len;

        symbolTable.write(s.getBytes("US-ASCII"));
    }

    public static void main(String[] args) throws Exception {
        String msg1 = "hello,";
        String msg2 = "world.";

        Test test = new Test();

        test.setup(true, false, 3);
        test.write(msg1);
        test.write(msg2);
        test.dump();
    }
}
