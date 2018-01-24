import static java.lang.System.out;

public class Main {
    private static final double DEFAULT_EPSILON = 0.1;
    private static final int DEFAULT_NP = 1;

    public static double f(double x) {
        return Math.sqrt(1 - Math.pow(x, 2));
    }

    private static double getEpsilon(String[] args) {
        if (args.length >= 1) {
            return Double.parseDouble(args[0]);
        }
        return DEFAULT_EPSILON;
    }

    private static int getNP(String[] args) {
        if (args.length >= 2) {
            return Integer.parseInt(args[1]);
        }
        return DEFAULT_NP;
    }

    private static double quad(double epsilon, double left, double right, double f_left, double f_right, double area) {
        double medium = (left + right) / 2;
        double f_medium = f(medium);
        double left_area = (f_left + f_medium) * (medium - left) / 2;
        double right_area = (f_medium + f_right) * (right - medium) / 2;

        if (Math.abs((left_area + right_area) - area) > epsilon) {
            left_area = quad(epsilon, left, medium, f_left, f_medium, left_area);
            right_area = quad(epsilon, medium, right, f_medium, f_right, right_area);
        }

        return (left_area + right_area);
    }

    private static double quad(double epsilon, int np, double a, double b) {
//        return quad(epsilon, a, b, f(a), f(b), (f(a) + f(b)) * (b - a) / 2);
        return new Quad(a, b, f(a), f(b), (f(a) + f(b)) * (b - a) / 2, epsilon).compute();
    }

    private static double calcPi(double epsilon, int np) {
        return quad(epsilon, np, 0, 1) * 4;
    }

    public static void main(String[] args) {
        double epsilon = getEpsilon(args);
        int np = getNP(args);

        out.println(calcPi(epsilon, np));
//        int[] arr = {11, 20, 12, 23, 14, 15, 21};
//        out.println(new CustomRecursiveTask(arr).compute());
    }
}
