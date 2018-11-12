import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Base64;
import java.util.Random;
import java.util.Scanner;

/**
 * User: Aksenov Vitaly
 * Date: 02.11.2018
 * Time: 12:28
 */
public class Interact {
    private final int KEY = 543515424;

    private final int MAX_TRIES = 3;

    private final int OK_EXIT_CODE = 0;
    private final int WA_EXIT_CODE = 10;

    private final String[] messages = { "Yes", "No", "N/A" };

    public int run(String infilename, String outfilename) throws IOException {
        try(Scanner in = new Scanner(new File(infilename))) {
            PrintWriter out = new PrintWriter(outfilename);
            try (Scanner pipeIn = new Scanner(System.in);
                 PrintWriter pipeOut = new PrintWriter(System.out)) {
                int seed = in.nextInt();
                Random rnd = new Random(seed);

                String message = messages[seed % 3];

                byte[] keyBytes = new byte[16];
                rnd.nextBytes(keyBytes);
                SecretKeySpec key = new SecretKeySpec(keyBytes, "AES");

                byte[] iv = new byte[16];
                rnd.nextBytes(iv);

                Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
                cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(iv));

                byte[] encrypted = cipher.doFinal(message.getBytes());

                pipeOut.println(Base64.getEncoder().encodeToString(encrypted));
                pipeOut.println(Base64.getEncoder().encodeToString(iv));
                pipeOut.flush();

                for (int i = 0; i < MAX_TRIES; i++) {
                    try {
                        String retry = pipeIn.next();
                        if (!retry.equals("YES") && !retry.equals("NO")) {
                            throw new Exception();
                        }

                        if (retry.equals("YES")) {
                            String ans = pipeIn.next();
                            if (message.equals(ans)) {
                                System.err.println("Here");
                                return writeResult(out, "OK", "Ok", OK_EXIT_CODE);
                            } else {
                                return writeResult(out, "WA", "Wrong answer", WA_EXIT_CODE);
                            }
                        }

                        byte[] toDecrypt = Base64.getDecoder().decode(pipeIn.next());
                        iv = Base64.getDecoder().decode(pipeIn.next());
                        cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
                        cipher.init(Cipher.DECRYPT_MODE, key, new IvParameterSpec(iv));;
                        cipher.doFinal(toDecrypt);
                        pipeOut.println("Ok");
                        pipeOut.flush();
                    } catch (BadPaddingException e) {
                        pipeOut.println("Wrong padding");
                        pipeOut.flush();
                    } catch (Exception e) {
                        return writeResult(out, "WA", "Protocol is broken", WA_EXIT_CODE);
                    }
                }

                return writeResult(out, "WA", "Out of queries", WA_EXIT_CODE);
            } catch (Exception e) {
                return writeResult(out, "WA", "Protocol is broken", WA_EXIT_CODE);
            }
        }
    }

    public int writeResult(PrintWriter out, String outcome, String comment, int exitCode) {
        out.println(KEY);
        out.println(outcome);
        out.println(comment);
        out.close();
        return exitCode;
    }

    public static void main(String[] args) throws IOException {
        System.exit(
        new Interact().run(
                args.length >= 2 ? args[0] : "input.txt",
                args.length >= 2 ? args[1] : "output.txt"
        )
        );
    }
}
