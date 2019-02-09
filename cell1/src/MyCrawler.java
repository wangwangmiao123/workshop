import java.awt.image.MemoryImageSource;
import java.io.Closeable;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Scanner;

import javax.swing.DebugGraphics;
import javax.swing.ScrollPaneConstants;
import javax.swing.plaf.synth.SynthSplitPaneUI;

import org.jsoup.Connection;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import de.l3s.boilerpipe.extractors.ArticleExtractor;
import de.l3s.boilerpipe.sax.BoilerpipeHTMLContentHandler;
import de.l3s.boilerpipe.sax.BoilerpipeHTMLParser;

public class MyCrawler {
	private int MAX_DEPTH = 2; 
	final String indexFileName ;
	final String  root_url ;
	private HashSet<String> link_history ; // links that 
	private IndexConstructor indexConstructor ;
	
	public MyCrawler(String init_url, String init_dir) {
		root_url = testInitUrl(init_url) ? init_url: "http://www.zju.edu.cn";
		indexFileName=init_dir; 
		link_history  = new HashSet<String>() ;		
		indexConstructor  =  new IndexConstructor(indexFileName) ;			
	}
	
	private boolean testInitUrl(String url) {
		try {
			Connection testConnection = Jsoup.connect(url);
		}catch (Exception e) {
			System.out.println("ERROR::Invalid URL,Fail to connect");
			return false ;
		}
		return true; 
	}
	
	void establish(){
		indexConstructor.createIndex();
		String  url = root_url;
		ArrayDeque<String> urlQueue  = new ArrayDeque<String>() ; 
		urlQueue.add(url);
		urlQueue.add("-") ; // Layer Separator  
		link_history.add(url); 
		int depth =0; // initial depath 
		ArrayList<String> temp = new ArrayList<String>(); //store links in each webpage 
		Document document ;
		String temp_string ;
		while(true) {
			if(depth>=MAX_DEPTH) {
				System.out.println("Max depth reached! Return");
				break;
			}
			
			url = urlQueue.poll() ;
			if(url.equals("-")) {
				depth ++ ;
				urlQueue.add("-");
				url= urlQueue.poll();  
				if(url.equals("-")) {
					break ; // if there is no valid  url  between 2 layer, return 
				}
			}
			
			//debug
			System.err.println("Under This Url");
			System.out.println(url);			
			
			try {
				document = Jsoup.connect(url).userAgent("Mozillar/5.0(jsoup)").get();
				temp_string = ArticleExtractor.INSTANCE.getText(document.html()); //extract the content of a webpage
				if (temp_string.length()>0) {
//					Debug
//					System.out.println("Content::::::");
//					System.out.println(temp_string);
				}
				link_history.add(url) ; //update crawl history 
			}
			catch(Exception e) {
				System.out.println("ERRROR::Fail to fetch webpage data");
				link_history.add(url) ; //update history anaway 
				continue ; //continue 
			}
			//debug
			//assert (indexConstructor) != null;
			if( temp_string.length() > 0 ) {
				indexConstructor.addContent(url,temp_string);
				//debug 
				System.out.printf("At layer : %d\n" ,depth);
			}
			
			try {
				temp =ExtractLinks.extractLinks(document) ; //extract links from 				
			}
			catch(Exception ex)
			{
				System.out.println(ex.getMessage());
				System.out.printf("bad url : %s ", url);
				continue; 
			}	
			
			for (String s_url : temp) {
				if(link_history.contains(s_url)) continue ;
				urlQueue.add(s_url); 
 			}
			indexConstructor.commit(); // write to disk
		}
		//debug 
		System.out.println("Control reach here!");
		indexConstructor.close(); 
	}
	
	void query(String queryString) {
		///sample query 
		try {
			indexConstructor.search(queryString);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	void showHistory() {
		System.out.println("START::Print Crawl History");
		for(String string : link_history) {
			System.out.println(string);
		}
	}
	
	void updateMaxDepth(int maxdepth) {
		if(maxdepth>0 && maxdepth<30) {
			MAX_DEPTH = maxdepth ;
		}
	}
	public static void main(String[] args) {
		String defaultUrl = "http://www.zju.edu.cn"; 
		String defaultIndexDir = "index" ;  
		java.util.Scanner  scanner = new java.util.Scanner(System.in);
		MyCrawler myCrawler ;

		System.out.println("Query or Crawl? 0-Crawl, 1-Search");
		int x = scanner.nextInt();
		if(x!=0 && x!=1) {
			System.out.println("bye~");
			return ;
		}
		System.out.println("Default Index Directory? 0-YES, 1-NO");
		int y = scanner.nextInt();
		if(y!=0 && y!=1) {
			System.out.println("bye~");
			return ;
		}
		if(1==y) {
			System.out.println("Input Index Directory Name:");
			String indexDir = scanner.next() ;
			if(null == indexDir) {
				System.out.println("Bye~");
			}
			defaultIndexDir = indexDir; 
		}
		if(0==x) {
			//crawl 
			System.out.println("Input Website URL to Crawl");
			String url= scanner.next();
			if(null == url) { 
				System.out.println("	You input an invalid URL ");
			}else {
				//debug
//				System.out.println(url);
				defaultUrl = url ;
			}
			myCrawler = new MyCrawler(defaultUrl,defaultIndexDir) ;
			System.out.println("Default max depth ?0-YES, 1-NO");
			int m = scanner.nextInt();
			if(1==m) {
				System.out.println("Input depth : ");
				int maxdepth = scanner.nextInt();
				myCrawler.updateMaxDepth(maxdepth); 
			}
			myCrawler = new MyCrawler(defaultUrl,defaultIndexDir) ;
			myCrawler.establish(); // establish index writer, crawl the website
			System.out.println("Show History? 0-YES, 1-NO");
			int k = scanner.nextInt(); 
			if(0==k) {
				myCrawler.showHistory(); 
			}
		
			System.out.println("Query Now? 0-YES, 1-NO");
			int z = scanner.nextInt() ;
			if(z==0) {
				//
			}else {
				if(x!=1) System.err.println("You input a invalid command, quit now !");
				return ; 
			}
		}else {
			myCrawler = new MyCrawler(defaultUrl,defaultIndexDir) ;
		}

		System.out.println("Please input query string");
		System.out.println("Query string format : ");
		System.out.print("BNF, the query grammar is:\n"+"Query  ::= ( Clause )*\n"+
			   "Clause ::= [\"+\", \"-\"] [<TERM> \":\"] ( <TERM> | \"(\" Query \")\" \n");
		String queryString = scanner.next();
		myCrawler.query(queryString);
		System.out.println("CRAWLER END");
 	}
}
