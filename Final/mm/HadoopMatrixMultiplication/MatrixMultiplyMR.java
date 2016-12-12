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


	//Mapper's 4 args are KEYIN,VALUEIN,KEYOUT,VALUEOUT
	//KEYIN doesn't matter, since it is just the byte offset in the file
	public static class MatrixMapper extends Mapper<Object, Text, Text, Text>{
		
		/*
			Object key: ignored, simply the byte offset in the file
			Text value: a line in the input file representing a value
						in one cell of the matrix.
		*/
		public void map(Object key, Text value, Context context) 
						throws IOException, InterruptedException {

			//Read dimension from config
			Configuration conf = context.getConfiguration();
			String dimension = conf.get("dimension");
			int dim = Integer.parseInt(dimension);

			//Read value and turn it into an array
			//If value is a, 0, 0, 1 then
			//inputArr[0] gives the name of the matrix
			//inputArr[1] gives the row
			//inputArr[2] gives the col
			//inputArr[3] gives value stored at that row, col 
			String inputLine = value.toString();
			String[] inputArr = inputLine.split(",");
			String matrixName = inputArr[0].trim();
			String row = inputArr[1].trim();
			String col = inputArr[2].trim();


			//This will be the intermediate key emitted by this map function
			String nKey = "";

			if(matrixName.equals("a")){
				for(int k=0; k<dim; k++){
					nKey = row+","+k;
					context.write(new Text(nKey), value);
				}
			}
			if(matrixName.equals("b")){
				for(int i=0; i<dim; i++){
					nKey = i+","+col;
					context.write(new Text(nKey), value);
				}
			}

		}
	}

	//Reducer's 4 args are KEYIN,VALUEIN,KEYOUT,VALUEOUT
	public static class MatrixReducer extends Reducer<Text, Text, Text, IntWritable>{

		public void reduce(Text key, Iterable<Text> values, Context context) 
							throws IOException, InterruptedException{

			//Read dimension from config
			Configuration conf = context.getConfiguration();
			String dimension = conf.get("dimension");
			int dim = Integer.parseInt(dimension);

			//This will be filled in with the relevant row and column to 
			//compute a given cell in matrix c after the multiplication.
			int[] row = new int[dim];
			int[] col = new int[dim];

			for(Text value: values){
				String valueLine = value.toString();
				String[] valueArr = valueLine.split(",");
				String matrixName = valueArr[0].trim();
				int indexInRow = Integer.parseInt(valueArr[2].trim());
				int indexInCol = Integer.parseInt(valueArr[1].trim());
				
				//Fill in the row and col array so we can perform matrix multiplication
				if(matrixName.equals("a")){
					row[indexInRow] = Integer.parseInt(valueArr[3].trim());
				}
				if(matrixName.equals("b")){
					col[indexInCol] = Integer.parseInt(valueArr[3].trim());
				}
			}

			//Do the matrix multiplication using row and col
			int cValue = 0;
			for(int n=0; n<dim; n++){
				cValue+=row[n]*col[n];
			}
			context.write(key, new IntWritable(cValue));
		}
	}

	public static void main(String[] args) throws Exception{
		Configuration conf = new Configuration();
		conf.set("dimension", "5"); //matrix dimensions
		Job job = Job.getInstance(conf, "matrix multiply");

		job.setJarByClass(MatrixMultiplyMR.class);
		job.setMapperClass(MatrixMapper.class);
		job.setReducerClass(MatrixReducer.class);

		//The map output key-value looks like:
		//0,0 - a, 0, 0, 1
		//where 0,0 is the text key and a, 0, 0, 1 is the text value
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(Text.class);

		//The reduce output key-value looks like:
		//0,0 - 4
		//where 0,0 is the text key and 4 is the integer value
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
				
		//Everything being read from the input files
		//and everything being written to output files are
		//in text format		
		job.setInputFormatClass(TextInputFormat.class);
		job.setOutputFormatClass(TextOutputFormat.class);
				
		//Make path variables so I can check them below.
		Path input = new Path(args[0]);
		Path output = new Path(args[1]);
		

		//Nice to have so it won't throw an error from lack of
		//input folder or actually having output folder 
		FileSystem fs = FileSystem.get(conf);
		if(!fs.exists(input)){
			System.err.println("No input folder provided.");
		}
		if(fs.exists(output)){
			fs.delete(output, true);
			System.out.println("Existing output folder was deleted");
		}
		fs.close();

		//setup path to input and outputs, which will be in 
		//InputFormatClass and OutputFormatClass formats
		FileInputFormat.addInputPath(job, input);
		FileOutputFormat.setOutputPath(job, output);
			
		//Boilerplate to check if job is done		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}