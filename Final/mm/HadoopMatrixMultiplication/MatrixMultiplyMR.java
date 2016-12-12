import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;

public class MatrixMultiplyMR{




	public static void main(String[] args) throws Exception{
		Configuration conf = new Configuration();
		conf.set("dimension", "1000"); //matrix dimensions
		Job job = Job.getInstance(conf, "matrix multiply");

		job.setJarByClass(MatrixMultiplyMR.class);
		job.setMapperClass(Matrix_Mapper.class);
		job.setReducerClass(Matrix_Reducer.class);



		/* Nice to have so it won't throw an error from lack of
		input folder or actually having output folder */
		FileSystem fs = FileSystem.get(conf);
		if(!fs.exists(input)){
			System.err.println("No input folder provided.");
		}
		if(fs.exists(output)){
			fs.delete(output, true);
			System.out.println("Existing output folder was deleted");
		}
		fs.close();

	}
}