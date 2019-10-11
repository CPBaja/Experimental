import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.awt.Color;
import java.util.ArrayList;
import java.io.PrintWriter;

final class ReadImageFile {

	private static final String imagePath = "C:\\Users\\Tyler Davis\\Desktop\\pan_edited.jpg";
	private static final String textPath = "C:\\Users\\Tyler Davis\\Desktop\\out.txt";

	private static final int INTEGER_SIZE = 16;

	public static void main(String[] args) {
		BufferedImage image = getImageFile();
		ArrayList<Integer> colors = new ArrayList<>();
		int height = image.getHeight(); int width = image.getWidth();
		for (int idx = 0; idx < height * width; idx++) {
			int row = idx / width; int col = idx % width;
			Color color = new Color(image.getRGB(col, row));
			int average = (color.getRed() + color.getGreen() + color.getBlue()) / 3;
			if (average >= 128)
				colors.add(1);
			else
				colors.add(0);
		}
		ArrayList<ArrayList<Integer>> listOfBinaryInt = group(colors);
		ArrayList<Integer> listOfShort= new ArrayList<>();
		for (ArrayList<Integer> binaryInt : listOfBinaryInt)
			listOfShort.add(binaryListToSignedShort(binaryInt));
		PrintWriter out = getOutputFile();
		out.print(listOfShort);
		out.close();
	}

	private static BufferedImage getImageFile() {
		BufferedImage image = null;
		try {
			image = ImageIO.read(new File(imagePath));
		} catch (IOException e) {
			System.exit(1);
		}
		return image;
	}

	private static ArrayList<ArrayList<Integer>> group(ArrayList<Integer> ungrouped) {
		ArrayList<ArrayList<Integer>> grouped = new ArrayList<>();
		for (int idx = 0; idx < ungrouped.size(); idx += INTEGER_SIZE) {
			ArrayList<Integer> group = new ArrayList<>();
			for (int i = idx; i < idx + INTEGER_SIZE; i++)
				group.add(ungrouped.get(i));
			grouped.add(group);
		}
		return grouped;
	}

	private static int binaryListToSignedShort(ArrayList<Integer> binaryList) {
		short unsignedInt = 0;
		for (int i = 0; i < INTEGER_SIZE; i++) {
			unsignedInt += binaryList.get(i) * Math.pow(2, INTEGER_SIZE - i - 1);
		}
		return unsignedInt;
	}

	private static PrintWriter getOutputFile() {
		PrintWriter out = null;
		try {
			out = new PrintWriter(new FileWriter(textPath));
		} catch (IOException e) {
			System.exit(1);
		}
		return out;
	}
}