import org.apache.xerces.util.EntityResolver2Wrapper;
import org.jsoup.Connection;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.IOException;
import java.util.ArrayList;

import javax.print.attribute.standard.MediaName;

final class ExtractLinks {
	public static ArrayList<String> extractLinks(Document doc) throws IOException{
		ArrayList<String> result = new ArrayList<String>();		
		Elements links = doc.select("a[href]")  ;
		for (Element link : links) {
			result.add(new String(link.attr("abs:href")));
	    }
		System.out.printf("result : %d \n", result.size()); 
		//debug 
		//for(String string : result)
		//{
		//	System.out.println(string);
		//}
		return result ;
	}
	
// Test
//	public static void main(String[] args) {
//		try {
//			ExtractLinks.extractLinks(Jsoup.connect("http://www.zju.edu.cn").userAgent("Mozillar/5.0(jsoup)").get());
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
//	}
}