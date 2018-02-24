public class Driver {
    public static void main(String[] args) {
        int numOfMen = 8;
        int numOfWomen = 8;
        int numberOfVisits = 6;

        Bathroom bathroom = new Bathroom(numOfMen, numOfWomen);

        for (int i = 0; i < numOfMen; i++) {
            Man man = new Man(bathroom, numberOfVisits, i);
            new Thread(man).start();
        }
        for (int i = 0; i < numOfWomen; i++) {
            Woman woman = new Woman(bathroom, numberOfVisits, i);
            new Thread(woman).start();
        }
    }
}
