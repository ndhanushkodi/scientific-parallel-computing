package mm;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class Matrix_Mapper extends Mapper<LongWritable,Text,Text,Text>
{
	
	/**
	 * Map function will collect/ group cell values required for 
	 * calculating the output. 
	 * @param key is ignored. Its just the byte offset
	 * @param value is a single line. (a, 0, 0, 63) (matrix name, row, column, value) 
	 * 
	 */
	
	@Override
	protected void map(LongWritable key, Text value,Context context)
						throws IOException, InterruptedException
	{
		System.out.println("Inside Map !");
		String line = value.toString();
		String[] entry = line.split(",");
		String sKey = "";
		String mat = entry[0].trim();
		
		String row, col;
		
		Configuration conf = context.getConfiguration();
		String dimension = conf.get("dimension");
		
		System.out.println("Dimension from Mapper = " + dimension);
		
		int dim = Integer.parseInt(dimension);
		
		
		if(mat.matches("a"))
		{
			for (int i =0; i < dim ; i++) // hard coding matrix size 5 
			{
				row = entry[1].trim(); // rowid
				sKey = row+","+i;
				System.out.println(sKey + "-" + value.toString());
				context.write(new Text(sKey),value);
			}
		}
		
		if(mat.matches("b"))
		{
			for (int i =0; i < dim ; i++)
			{
				col = entry[2].trim(); // colid
				sKey = i+col;
				System.out.println(sKey + "-" + value.toString());
				context.write(new Text(sKey),value);
			}
		}
		
	}
	

}
