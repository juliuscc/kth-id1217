import java.util.Arrays;
import java.util.Collection;
import java.util.concurrent.ForkJoinTask;
import java.util.concurrent.RecursiveTask;
import java.util.ArrayList;
import java.util.List;

public class Quad extends RecursiveTask<Double> {
    private double f(double x) {
        return Math.sqrt(1 - Math.pow(x, 2));
    }

    double left;
    double right;
    double f_left;
    double f_right;
    double area;
    double epsilon;

    public Quad(double left, double right, double f_left, double f_right, double area, double epsilon) {
        this.left = left;
        this.right = right;
        this.f_left = f_left;
        this.f_right = f_right;
        this.area = area;
        this.epsilon = epsilon;
    }

    @Override
    protected Double compute() {
        double medium = (left + right) / 2;
        double f_medium = f(medium);
        double left_area = (f_left + f_medium) * (medium - left) / 2;
        double right_area = (f_medium + f_right) * (right - medium) / 2;
        
        if (Math.abs((left_area + right_area) - area) > epsilon) {
            Quad left_quad = new Quad(left, medium, f_left, f_medium, left_area, epsilon);
            Quad right_quad = new Quad(medium, right, f_medium, f_right, right_area, epsilon);
            List<Quad> dividedTasks = new ArrayList<>();
            dividedTasks.add(left_quad);
            dividedTasks.add(right_quad);
            return ForkJoinTask.invokeAll(dividedTasks)
                    .stream()
                    .mapToDouble(ForkJoinTask::join)
                    .sum();
        }

        return (left_area + right_area);
    }


//    private Collection<CustomRecursiveTask> createSubtasks() {
//        List<CustomRecursiveTask> dividedTasks = new ArrayList<>();
//
//        dividedTasks.add(new CustomRecursiveTask(
//                Arrays.copyOfRange(arr, 0, arr.length / 2))
//        );
//        dividedTasks.add(new CustomRecursiveTask(
//                Arrays.copyOfRange(arr, arr.length / 2, arr.length))
//        );
//
//        return dividedTasks;
//    }
//
//    private Integer processing(int[] arr) {
//        return Arrays.stream(arr)
//                .filter(a -> a > 10 && a < 27)
//                .map(a -> a * 10)
//                .sum();
//    }
}
