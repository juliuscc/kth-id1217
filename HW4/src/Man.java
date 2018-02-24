import java.util.Random;

public class Man implements Runnable {

    private Random r = new Random();
    private Bathroom bathroom;
    private int numberOfVisits;
    public int visits = 0;
    public int id;

    public Man(Bathroom bathroom, int numberOfVisits, int id) {
        this.bathroom = bathroom;
        this.numberOfVisits = numberOfVisits;
        this.id = id;
    }

    public void run() {
        try {
            while (visits < numberOfVisits) {
                /* Work up to 5 seconds */
                Thread.sleep(r.nextInt(5000));

                /* Enter or wait to enter the bathroom */
                bathroom.manEnter(this);

                /* Go to the bathroom up to 2 seconds*/
                Thread.sleep(r.nextInt(2000));

                /* Exit the bathroom */
                bathroom.manExit(this);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new RuntimeException(e);
        }
    }
}
