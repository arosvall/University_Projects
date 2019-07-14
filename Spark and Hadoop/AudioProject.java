package AudioProject;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.types.StructType;
import org.apache.spark.ml.classification.RandomForestClassificationModel;
import org.apache.spark.ml.classification.RandomForestClassifier;
import org.apache.spark.ml.evaluation.MulticlassClassificationEvaluator;
import org.apache.spark.sql.functions;
import org.apache.spark.ml.feature.VectorAssembler;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.ml.feature.Normalizer;
import org.apache.spark.ml.feature.PCA;
import org.apache.spark.ml.feature.StandardScaler;

public class AudioProject {

	private static int seed = 0;

	public AudioProject() {

		SparkSession spark = SparkSession.builder().appName("AP").getOrCreate();

		long startTime = System.currentTimeMillis();

		StructType schema = this.createSchema();

		Dataset<Row> data = spark.read().format("csv").option("header", "true").schema(schema)
				.load("year_prediction.csv");
		//Call the assemble data method to get the features column created and delete the other columns as they are no longer useful
		Dataset<Row> vector_data = this.assembleData(data);
		//Now balance the dataset, change the year labels to 0-15 class numbering, filter out data which does not have as many instances
		vector_data = this.preprocessData(vector_data);
		long endTime = System.currentTimeMillis();
		System.out.println("Time spent loading and preprocessing: " + (endTime - startTime) + " ms");

		//Now build and run the models
		this.baseModel(vector_data);
		this.pcaModel(vector_data);
		this.normalisedModel(vector_data);
	}

	private void baseModel(Dataset<Row> data) {
		long startTime = System.currentTimeMillis();
		System.out.print("Base model");
		//split the data into 30 test 70 training using the static seed
		Dataset<Row>[] splits = data.randomSplit(new double[] { 0.7, 0.3 }, seed);
		Dataset<Row> training = splits[0];
		Dataset<Row> test = splits[1];

		RandomForestClassifier rf = new RandomForestClassifier().setNumTrees(5).setMaxDepth(12);
		RandomForestClassificationModel rfModel = rf.fit(training);

		Dataset<Row> rfpredictions = rfModel.transform(training);
		evaluateModel(rfpredictions, true);

		//find the test accuracy
		rfpredictions = rfModel.transform(test);
		evaluateModel(rfpredictions,false);
		long endTime = System.currentTimeMillis();
		System.out.println("Time spent creating and evaluating base model: " + (endTime - startTime) + " ms");
	}

	private void pcaModel(Dataset<Row> data) {
		long startTime = System.currentTimeMillis();
		System.out.print("PCA model");
		//create the principal components

		Dataset<Row> pca_result = new PCA().setInputCol("features").setOutputCol("pcaFeatures").setK(20).fit(data).transform(data).drop("features");
		pca_result = pca_result.withColumnRenamed("pcaFeatures", "features");

		Dataset<Row>[] splits = pca_result.randomSplit(new double[] { 0.7, 0.3 }, seed);
		Dataset<Row> training = splits[0];
		Dataset<Row> test = splits[1];

		RandomForestClassifier rf = new RandomForestClassifier().setNumTrees(5).setMaxDepth(12);
		RandomForestClassificationModel rfModel = rf.fit(training);

		Dataset<Row> rfpredictions = rfModel.transform(training);
		evaluateModel(rfpredictions, true);

		//find the test accuracy
		rfpredictions = rfModel.transform(test);
		evaluateModel(rfpredictions,false);

		long endTime = System.currentTimeMillis();
		System.out.println("Time spent creating and evaluating pca model: " + (endTime - startTime) + " ms");
	}

	private void normalisedModel(Dataset<Row> data) {
		long startTime = System.currentTimeMillis();
		System.out.print("Normalised model");

		Dataset<Row> scaled_data = new Normalizer()
	            .setInputCol("features").setOutputCol("scaledFeatures")
	            .transform(data)
	            .drop("features");
		scaled_data = scaled_data.withColumnRenamed("scaledFeatures", "features");
		Dataset<Row>[] splits = scaled_data.randomSplit(new double[] { 0.7, 0.3 }, seed);
		Dataset<Row> training = splits[0];
		Dataset<Row> test = splits[1];

		RandomForestClassifier rf = new RandomForestClassifier().setNumTrees(5).setMaxDepth(12);
		RandomForestClassificationModel rfModel = rf.fit(training);

		Dataset<Row> rfpredictions = rfModel.transform(training);
		evaluateModel(rfpredictions, true);

		//find the test accuracy
		rfpredictions = rfModel.transform(test);
		evaluateModel(rfpredictions,false);
		long endTime = System.currentTimeMillis();
		System.out.println("Time spent creating and evaluating base model: " + (endTime - startTime) + " ms");
	}

	private void evaluateModel(Dataset<Row> predictions, boolean b) {
		MulticlassClassificationEvaluator eval = new MulticlassClassificationEvaluator().setLabelCol("label")
				.setPredictionCol("prediction").setMetricName("accuracy");
		double accuracy = eval.evaluate(predictions);
		eval = new MulticlassClassificationEvaluator().setLabelCol("label")
				.setPredictionCol("prediction").setMetricName("f1");
		double f1 = eval.evaluate(predictions);
		eval = new MulticlassClassificationEvaluator().setLabelCol("label")
				.setPredictionCol("prediction").setMetricName("weightedPrecision");
		double precis = eval.evaluate(predictions);
		eval = new MulticlassClassificationEvaluator().setLabelCol("label")
				.setPredictionCol("prediction").setMetricName("weightedRecall");
		double recall = eval.evaluate(predictions);

		System.out.println(" /************************/**********************/");
		if (b == true) {
			System.out.println("Training accuracy = " + accuracy);
			System.out.println("Training f1 = " + f1);
			System.out.println("Training recall = " + recall);
			System.out.println("Training precision = " + precis);

		} else {
			System.out.println("Test accuracy = " + accuracy);
			System.out.println("Test f1 = " + f1);
			System.out.println("Test recall = " + recall);
			System.out.println("Test precision = " + precis);

		}
		System.out.println(" /************************/**********************/");
	}

	private Dataset<Row> preprocessData(Dataset<Row> vector_data) {
		vector_data = vector_data.filter("label > 2005");
		vector_data = vector_data.filter("label < 2008");

		Dataset<Row> df_2006 = vector_data.filter("label = 2006");
		Dataset<Row> df_2007 = vector_data.filter("label = 2007");

		int count = (int) df_2006.count();
    	double	ratio = ((count * 1.0) / df_2007.count());
		df_2007 = df_2007.sample(false, ratio, seed).limit(count);
		vector_data = df_2007.union(df_2006);

		vector_data = vector_data.withColumn("label", functions.when(vector_data.col("label").equalTo("2006"), 0).when(vector_data.col("label").equalTo("2007"), 1));

		vector_data = new StandardScaler().setInputCol("features").setOutputCol("scaledFeatures")
	            .fit(vector_data).transform(vector_data)
	            .drop("features");

		vector_data = vector_data.withColumnRenamed("scaledFeatures", "features");
		return vector_data;
	}

	private Dataset<Row> assembleData(Dataset<Row> data) {
		String[] featureList = new String[] { "TimbreAvg1", "TimbreAvg2", "TimbreAvg3", "TimbreAvg4", "TimbreAvg5",
				"TimbreAvg6", "TimbreAvg7", "TimbreAvg8", "TimbreAvg9", "TimbreAvg10", "TimbreAvg11", "TimbreAvg12",
				"TimbreCovariance1", "TimbreCovariance2", "TimbreCovariance3", "TimbreCovariance4", "TimbreCovariance5",
				"TimbreCovariance6", "TimbreCovariance7", "TimbreCovariance8", "TimbreCovariance9",
				"TimbreCovariance10", "TimbreCovariance11", "TimbreCovariance12", "TimbreCovariance13",
				"TimbreCovariance14", "TimbreCovariance15", "TimbreCovariance16", "TimbreCovariance17",
				"TimbreCovariance18", "TimbreCovariance19", "TimbreCovariance20", "TimbreCovariance21",
				"TimbreCovariance22", "TimbreCovariance23", "TimbreCovariance24", "TimbreCovariance25",
				"TimbreCovariance26", "TimbreCovariance27", "TimbreCovariance28", "TimbreCovariance29",
				"TimbreCovariance30", "TimbreCovariance31", "TimbreCovariance32", "TimbreCovariance33",
				"TimbreCovariance34", "TimbreCovariance35", "TimbreCovariance36", "TimbreCovariance37",
				"TimbreCovariance38", "TimbreCovariance39", "TimbreCovariance40", "TimbreCovariance41",
				"TimbreCovariance42", "TimbreCovariance43", "TimbreCovariance44", "TimbreCovariance45",
				"TimbreCovariance46", "TimbreCovariance47", "TimbreCovariance48", "TimbreCovariance49",
				"TimbreCovariance50", "TimbreCovariance51", "TimbreCovariance52", "TimbreCovariance53",
				"TimbreCovariance54", "TimbreCovariance55", "TimbreCovariance56", "TimbreCovariance57",
				"TimbreCovariance58", "TimbreCovariance59", "TimbreCovariance60", "TimbreCovariance61",
				"TimbreCovariance62", "TimbreCovariance63", "TimbreCovariance64", "TimbreCovariance65",
				"TimbreCovariance66", "TimbreCovariance67", "TimbreCovariance68", "TimbreCovariance69",
				"TimbreCovariance70", "TimbreCovariance71", "TimbreCovariance72", "TimbreCovariance73",
				"TimbreCovariance74", "TimbreCovariance75", "TimbreCovariance76", "TimbreCovariance77",
				"TimbreCovariance78" };

		String[] topFeatures = new String[] {"TimbreAvg1","TimbreAvg3","TimbreAvg6","TimbreAvg7","TimbreAvg11","TimbreCovariance9","TimbreCovariance12","TimbreCovariance15","TimbreCovariance16","TimbreCovariance17","TimbreCovariance21","TimbreCovariance22","TimbreCovariance23","TimbreCovariance25","TimbreCovariance26","TimbreCovariance28","TimbreCovariance31","TimbreCovariance34","TimbreCovariance35","TimbreCovariance38","TimbreCovariance41","TimbreCovariance42","TimbreCovariance45","TimbreCovariance47","TimbreCovariance50","TimbreCovariance51","TimbreCovariance53","TimbreCovariance54","TimbreCovariance55","TimbreCovariance56","TimbreCovariance57","TimbreCovariance58","TimbreCovariance61","TimbreCovariance62","TimbreCovariance66","TimbreCovariance70","TimbreCovariance73","TimbreCovariance74","TimbreCovariance76"};

		VectorAssembler assembler = new VectorAssembler().setInputCols(topFeatures).setOutputCol("features");
		Dataset<Row> vector_data = assembler.transform(data);
		vector_data = vector_data.drop(featureList);
		return vector_data;
	}

	private StructType createSchema() {
		return new StructType().add("label", "int").add("TimbreAvg1", "double").add("TimbreAvg2", "double")
				.add("TimbreAvg3", "double").add("TimbreAvg4", "double").add("TimbreAvg5", "double")
				.add("TimbreAvg6", "double").add("TimbreAvg7", "double").add("TimbreAvg8", "double")
				.add("TimbreAvg9", "double").add("TimbreAvg10", "double").add("TimbreAvg11", "double")
				.add("TimbreAvg12", "double").add("TimbreCovariance1", "double").add("TimbreCovariance2", "double")
				.add("TimbreCovariance3", "double").add("TimbreCovariance4", "double")
				.add("TimbreCovariance5", "double").add("TimbreCovariance6", "double")
				.add("TimbreCovariance7", "double").add("TimbreCovariance8", "double")
				.add("TimbreCovariance9", "double").add("TimbreCovariance10", "double")
				.add("TimbreCovariance11", "double").add("TimbreCovariance12", "double")
				.add("TimbreCovariance13", "double").add("TimbreCovariance14", "double")
				.add("TimbreCovariance15", "double").add("TimbreCovariance16", "double")
				.add("TimbreCovariance17", "double").add("TimbreCovariance18", "double")
				.add("TimbreCovariance19", "double").add("TimbreCovariance20", "double")
				.add("TimbreCovariance21", "double").add("TimbreCovariance22", "double")
				.add("TimbreCovariance23", "double").add("TimbreCovariance24", "double")
				.add("TimbreCovariance25", "double").add("TimbreCovariance26", "double")
				.add("TimbreCovariance27", "double").add("TimbreCovariance28", "double")
				.add("TimbreCovariance29", "double").add("TimbreCovariance30", "double")
				.add("TimbreCovariance31", "double").add("TimbreCovariance32", "double")
				.add("TimbreCovariance33", "double").add("TimbreCovariance34", "double")
				.add("TimbreCovariance35", "double").add("TimbreCovariance36", "double")
				.add("TimbreCovariance37", "double").add("TimbreCovariance38", "double")
				.add("TimbreCovariance39", "double").add("TimbreCovariance40", "double")
				.add("TimbreCovariance41", "double").add("TimbreCovariance42", "double")
				.add("TimbreCovariance43", "double").add("TimbreCovariance44", "double")
				.add("TimbreCovariance45", "double").add("TimbreCovariance46", "double")
				.add("TimbreCovariance47", "double").add("TimbreCovariance48", "double")
				.add("TimbreCovariance49", "double").add("TimbreCovariance50", "double")
				.add("TimbreCovariance51", "double").add("TimbreCovariance52", "double")
				.add("TimbreCovariance53", "double").add("TimbreCovariance54", "double")
				.add("TimbreCovariance55", "double").add("TimbreCovariance56", "double")
				.add("TimbreCovariance57", "double").add("TimbreCovariance58", "double")
				.add("TimbreCovariance59", "double").add("TimbreCovariance60", "double")
				.add("TimbreCovariance61", "double").add("TimbreCovariance62", "double")
				.add("TimbreCovariance63", "double").add("TimbreCovariance64", "double")
				.add("TimbreCovariance65", "double").add("TimbreCovariance66", "double")
				.add("TimbreCovariance67", "double").add("TimbreCovariance68", "double")
				.add("TimbreCovariance69", "double").add("TimbreCovariance70", "double")
				.add("TimbreCovariance71", "double").add("TimbreCovariance72", "double")
				.add("TimbreCovariance73", "double").add("TimbreCovariance74", "double")
				.add("TimbreCovariance75", "double").add("TimbreCovariance76", "double")
				.add("TimbreCovariance77", "double").add("TimbreCovariance78", "double");
	}

	public static void main(String[] args) {
		System.out.println("start");
		new AudioProject();
	}

}
