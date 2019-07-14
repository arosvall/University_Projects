package mySpark.DT;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.ml.evaluation.MulticlassClassificationEvaluator;
import org.apache.spark.ml.feature.VectorAssembler;
import org.apache.spark.sql.Dataset;
import org.apache.spark.ml.classification.DecisionTreeClassifier;
import org.apache.spark.ml.classification.DecisionTreeClassificationModel;
import org.apache.spark.sql.Row;

public class DTree {

	public static void main(String[] args) {
		int count = 0;
		long[] seeds = { 123, 456, 789, 1000, 987, 123456, 1000100, 191919, 424424, 310519 };
	//	SparkSession spark = SparkSession.builder().master("local").appName("LogReg").config("spark-master", "local[2]")
	//			.getOrCreate();
		SparkSession spark = SparkSession.builder().appName("DT").getOrCreate();

		StructType schema = new StructType().add("_c0", "double").add("_c1", "double").add("_c2", "double")
				.add("_c3", "double").add("_c4", "double").add("_c5", "double").add("_c6", "double")
				.add("_c7", "double").add("_c8", "double").add("_c9", "double").add("_c10", "double")
				.add("_c11", "double").add("_c12", "double").add("_c13", "double").add("_c14", "double")
				.add("_c15", "double").add("_c16", "double").add("_c17", "double").add("_c18", "double")
				.add("_c19", "double").add("_c20", "double").add("_c21", "double").add("_c22", "double")
				.add("_c23", "double").add("_c24", "double").add("_c25", "double").add("_c26", "double")
				.add("_c27", "double").add("_c28", "double").add("_c29", "double").add("_c30", "double")
				.add("_c31", "double").add("_c32", "double").add("_c33", "double").add("_c34", "double")
				.add("_c35", "double").add("_c36", "double").add("_c37", "double").add("_c38", "double")
				.add("_c39", "double").add("label", "int");

		Dataset<Row> data = spark.read().format("csv").schema(schema).load("Kdd.csv");

		VectorAssembler assembler = new VectorAssembler().setInputCols(new String[] { "_c0", "_c1", "_c2", "_c3", "_c4",
				"_c4", "_c6", "_c7", "_c8", "_c9", "_c10", "_c11", "_c12", "_c13", "_c14", "_c15", "_c16", "_c17",
				"_c18", "_c19", "_c20", "_c21", "_c22", "_c23", "_c24", "_c25", "_c26", "_c27", "_c28", "_c29", "_c30",
				"_c31", "_c32", "_c33", "_c34", "_c35", "_c36", "_c37", "_c38", "_c39" }).setOutputCol("features");
		Dataset<Row> vector_data = assembler.transform(data);

		while (count != 10) {
			long startTime = System.currentTimeMillis();
			long s = seeds[count];

			Dataset<Row>[] splits = vector_data.randomSplit(new double[] { 0.7, 0.3 }, s);
			Dataset<Row> training = splits[0];
			Dataset<Row> test = splits[1];

			DecisionTreeClassifier dt = new DecisionTreeClassifier().setMaxDepth(5);
			DecisionTreeClassificationModel dtModel = dt.fit(training);

			Dataset<Row> predictionsTrain = dtModel.transform(training);
			predictionsTrain.show(5);

			MulticlassClassificationEvaluator evalTrain = new MulticlassClassificationEvaluator().setLabelCol("label")
					.setPredictionCol("prediction").setMetricName("accuracy");
			double accuracyTrain = evalTrain.evaluate(predictionsTrain);
			System.out.println(" /************************/**********************/");
			System.out.println("Training accuracy = " + accuracyTrain);
			System.out.println(" /************************/**********************/");

			Dataset<Row> predictions = dtModel.transform(test);
			predictions.show(5);

			MulticlassClassificationEvaluator eval = new MulticlassClassificationEvaluator().setLabelCol("label")
					.setPredictionCol("prediction").setMetricName("accuracy");
			double accuracy = eval.evaluate(predictions);
			System.out.println(" /************************/**********************/");

			System.out.println("Test accuracy = " + accuracy);
			System.out.println(" /************************/**********************/");

			long endTime = System.currentTimeMillis();
			System.out.println(" /************************/**********************/");
			System.out.println("Time taken for program run " + count + " is: " + (endTime - startTime) + " ms");
			System.out.println(" /************************/**********************/");

			count++;
		}
	}
}
