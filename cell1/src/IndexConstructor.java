import java.io.*;

import javax.sound.midi.SysexMessage;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;

import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.Term;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.queryparser.classic.QueryParserBase;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TermQuery;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;
import org.wltea.analyzer.lucene.IKAnalyzer;

public class IndexConstructor {
	IndexWriter iwr;
	IndexSearcher searcher; 
	String indexFilePath;
	
	public IndexConstructor(String file) {
		// TODO Auto-generated constructor stub
		indexFilePath = file;
		searcher = null ;
	}
	
	void close()  {
		try {
			iwr.commit();
			iwr.close(); 
			
		}catch (Exception e) {
			if(null==iwr) {
				System.err.println("ERROR::No Valid IndexWriter");
			}
		}
	}
	
	public void createIndex(){
		File f=new File(indexFilePath);
		iwr=null;
		try {
			Directory dir=FSDirectory.open(f);
			Analyzer analyzer = new IKAnalyzer();			
			IndexWriterConfig conf=new IndexWriterConfig(Version.LUCENE_4_10_0,analyzer);
			iwr=new IndexWriter(dir,conf); 
		} catch (IOException e) {
			System.err.println("ERROR::Fail to create index");
			e.printStackTrace();
		}
	}
	
	public void commit() {
		if(null!=iwr)
			try {
				iwr.commit();
			} catch (IOException e) {
				System.err.println("ERROR::IndexWriter Commit Fail");
			}
		else {
			createIndex();
			try {
				iwr.commit();
			} catch (IOException e) {	
				System.err.println("ERROR::Fail to create Index Writer");
			}
		}
	}
	
	public void addContent(String url, String content) {

		assert(iwr!=null);
		Document new_document=new Document(); 
		Field f1=new TextField("Url",url,Field.Store.YES);
		Field f2=new TextField("WebpageContent",content,Field.Store.YES);
		new_document.add(f1);
		new_document.add(f2);
		
		try {
//			System.out.println(new_document.toString());
			iwr.addDocument(new_document);
		} catch (Exception e) {
			System.out.println("Add content error");
			e.printStackTrace();
		}
	}
	
	public void search(String queryStr) throws IOException{
		if(searcher == null) {
			searcher=new IndexSearcher(DirectoryReader.open(FSDirectory.open(new File(indexFilePath))));
		}
		try {
			Analyzer analyzer = new IKAnalyzer();
			@SuppressWarnings("deprecation")
			QueryParser parser = new QueryParser(Version.LUCENE_4_10_0, "", analyzer);
//			QueryParser parser2= new QueryParser(matchVersion, f, a)
			
			Query query=parser.parse(queryStr);
//			searcher.search
			TopDocs hits=searcher.search(query,10);
			for(ScoreDoc doc:hits.scoreDocs){
				Document d=searcher.doc(doc.doc);
				System.out.println("++++++++++++");
				System.out.print("URL :  ");
				System.out.println(d.get("Url"));
				System.out.print("Content :   ");
				System.out.println(d.get("WebpageContent"));
			}
		} catch (IOException | ParseException e) {
			e.printStackTrace();
		}
	}
////	////////test
//	public static void main(String[] args) throws IOException {
//		System.out.print("BNF, the query grammar is:\n"+"Query  ::= ( Clause )*\n"+
//				   "Clause ::= [\"+\", \"-\"] [<TERM> \":\"] ( <TERM> | \"(\" Query \")\" \n");
//
//		IndexConstructor test = new IndexConstructor("index_dir"); 
//		test.createIndex();
//		test.addContent("123", "大家好，这里是玉泉");
//		test.addContent("hihwe", "Hi, everyone, here is Restaurant!");
//		test.addContent("wr2", "Welcome! ");
//		test.addContent("1af", "Hello");
//		test.close() ;
//		try { 
//			test.search("+WebpageContent:玉泉");
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
//		System.out.println();
//	}
}
