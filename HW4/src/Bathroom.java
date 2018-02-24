import java.util.Date;

public class Bathroom {
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_PURPLE = "\u001B[35m";
    public static final String ANSI_BLUE = "\u001B[34m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";

    public static final char BLACK_SQUARE = '\u25A0';
    public static final char WHITE_SQUARE = '\u25A1';

    private BathroomState state = BathroomState.Empty;

    private int numMen;
    private int numWomen;

    private int menInQueue = 0;
    private int menInBathroom = 0;
    private int womenInQueue = 0;
    private int womenInBathroom = 0;
    private int time = 0;

    public Bathroom(int numMen, int numWomen) {
        this.numMen = numMen;
        this.numWomen = numWomen;
    }

    private void printQueues() {
        String timeStamp = "";
        if (time < 10)
            timeStamp = "[ " + time++ + "  ] ";
        else if (time >= 10 && time < 100)
            timeStamp = "[ " + time++ + " ] ";
        else if (time >= 100 && time < 1000)
            timeStamp = "[" + time++ + " ] ";
        else
            timeStamp = "[" + time++ + "] ";

        String stateColour = "";
        String statePadded = "";
        switch (state) {
            case Empty:
                stateColour = ANSI_YELLOW;
                statePadded = "    Empty     ";
                break;
            case MenEntering:
                stateColour = ANSI_GREEN;
                statePadded = " Men Entering ";
                break;
            case MenLeaving:
                stateColour = ANSI_RED;
                statePadded = " Men Leaving  ";
                break;
            case WomenEntering:
                stateColour = ANSI_GREEN;
                statePadded = "Women Entering";
                break;
            case WomenLeaving:
                stateColour = ANSI_RED;
                statePadded = "Women Leaving ";
                break;
        }

        String stateString = stateColour + "State: [" + statePadded + "] " + ANSI_RESET;

        String menQueue = "[";
        for (int i = 0; i < menInQueue; i++)
            menQueue += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInQueue; i++)
            menQueue += WHITE_SQUARE;
        menQueue += "]";

        String womenQueue = "[";
        for (int i = 0; i < womenInQueue; i++)
            womenQueue += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInQueue; i++)
            womenQueue += WHITE_SQUARE;
        womenQueue += "]";

        String menBathroom = "[";
        for (int i = 0; i < menInBathroom; i++)
            menBathroom += BLACK_SQUARE;
        for (int i = 0; i < numMen - menInBathroom; i++)
            menBathroom += ' ';
        menBathroom += "]";

        String womenBathroom = "[";
        for (int i = 0; i < womenInBathroom; i++)
            womenBathroom += BLACK_SQUARE;
        for (int i = 0; i < numWomen - womenInBathroom; i++)
            womenBathroom += ' ';
        womenBathroom += "]";

        System.out.print(timeStamp + stateString + "Bathroom: " + ANSI_BLUE + "M:" + menBathroom + ANSI_PURPLE + "W:" + womenBathroom + ANSI_RESET + " Queues: " + ANSI_BLUE + "M:" + menQueue + ANSI_PURPLE + "W:" + womenQueue + ANSI_RESET + " ");


    }

    public synchronized void manEnter(Man man) throws InterruptedException {
        printQueues();
        System.out.println("\uD83D\uDEB9 " + man.id + " wants to enter");

        menInQueue++;
        while (state != BathroomState.Empty && state != BathroomState.MenEntering) {
            wait();
        }
        menInQueue--;
        state = BathroomState.MenEntering;
        menInBathroom++;

        printQueues();
        System.out.println("\uD83D\uDEB9 " + man.id + " enters. Visit: " + ANSI_BLUE + ++man.visits + ANSI_RESET);
    }

    public synchronized void manExit(Man man) {
        state = BathroomState.MenLeaving;
        menInBathroom--;

        printQueues();
        System.out.println("\uD83D\uDEB9 " + man.id + " leaves.");

        if (menInBathroom == 0) {
            if (womenInQueue > 0)
                state = BathroomState.WomenEntering;
            else if (menInQueue > 0)
                state = BathroomState.MenEntering;
            else
                state = BathroomState.Empty;
        }

        notifyAll();
    }

    public synchronized void womanEnter(Woman woman) throws InterruptedException {
        printQueues();
        System.out.println("\uD83D\uDEBA " + woman.id + " wants to enter.");

        womenInQueue++;

        while (state != BathroomState.Empty && state != BathroomState.WomenEntering) {
            wait();
        }
        womenInQueue--;
        state = BathroomState.WomenEntering;
        womenInBathroom++;

        printQueues();
        System.out.println("\uD83D\uDEBA " + woman.id + " enters. Visit: " + ANSI_PURPLE + ++woman.visits + ANSI_RESET);

    }


    public synchronized void womanExit(Woman woman) {
        state = BathroomState.WomenLeaving;
        womenInBathroom--;

        printQueues();
        System.out.println("\uD83D\uDEBA " + woman.id + " leaves.");

        if (womenInBathroom == 0) {
            if (menInQueue > 0)
                state = BathroomState.MenEntering;
            else if (womenInQueue > 0)
                state = BathroomState.WomenEntering;
            else
                state = BathroomState.Empty;
        }

        notifyAll();
    }
}
