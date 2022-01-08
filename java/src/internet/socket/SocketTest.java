package internet.socket;

import java.io.IOException;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

/**
 * This program makes a socket connection to the atomic clock in Boulder, Colorado, and prints
 * the time that the server sends.
 *
 * @author Cay Horstmann
 * @version 1.22 2018-03-17
 */
public class SocketTest {
    public static void main(String[] args) throws IOException {
        try (var s = new Socket("time-a.nist.gov", 13);
             var in = new Scanner(s.getInputStream(), StandardCharsets.UTF_8)) {
            while (in.hasNextLine()) {
                String line = in.nextLine();
                System.out.println(line);
            }
        }
    }
}
