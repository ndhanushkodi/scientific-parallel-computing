package mm;
import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class Matrix_Reducer extends Reducer<Text, Text, Text, IntWritable>
{

	/**
	 * Reducer do the actual matrix multiplication.
	 * @param key is the cell unique cell dimension (00) represents cell 0,0
	 * @value values required to calculate matrix multiplication result of that cell.
	 */
	
	@Override
	protected void reduce(Text key, Iterable<Text> values,Context context)
						throws IOException, InterruptedException 
	{
		
		Configuration conf = context.getConfiguration();
		String dimension = conf.get("dimension");
		
		int dim = Integer.parseInt(dimension);
		
		//System.out.println("Dimension from Reducer = " + dimension);
		
		int[] row = new int[dim]; // hard coding as 5 X 5 matrix
		int[] col = new int[dim];
		
		for(Text val : values)
		{
			String[] entries = val.toString().split(",");
			if(entries[0].matches("a"))
			{
				int index = Integer.parseInt(entries[2].trim());
				row[index] = Integer.parseInt(entries[3].trim());
			}
			if(entries[0].matches("b"))
			{
				int index = Integer.parseInt(entries[1].trim());
				col[index] = Integer.parseInt(entries[3].trim());
			}
		}
		
		// Let us do matrix multiplication now..
		int total = 0;
		for(int i = 0 ; i < 5; i++)
		{
			total += row[i]*col[i];
		}
		System.out.println(key.toString() + "-" + total );
		context.write(key, new IntWritable(total));
	
	}
	
}
