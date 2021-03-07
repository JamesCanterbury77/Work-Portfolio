import java.util.Scanner;
import java.io.*;
import java.lang.StringBuffer;

/*
 * This class implements a spell checker application. 
 * This class requires a proper implementation to the StirngSet class.
 */
public class SpellChecker {

  public static void main(String [] args) {

    File f = new File("dictionary");
    
    try {
      Scanner sk = new Scanner(f);
        
      StringSet x = new StringSet();
    
      // Read in the entire dictionary...
      while (sk.hasNext()) {
        String word = sk.next();
        x.insert(word);      
      }
      System.out.println("Dicitonary loaded...");
           
      sk = new Scanner(System.in);
      
      // Keep suggesting alternatives as long as the user makes an input.
      while (sk.hasNext()) {
        String word = sk.next();
        if (x.find(word))
	  			System.out.println(word + " is correct.");
        else {
	  			char[] alphabet = "abcdefghijklmnopqrstuvwxyz".toCharArray();
                System.out.println("Suggesting alternatives ...");
                    for (int i = 0; i < word.length(); i++) {
                        StringBuffer a = new StringBuffer(word);
                        for (int j = 0; j < 26; j++)
                        {
                            char c = alphabet[j];
                            a.setCharAt(i, c);
                            if (x.find(a.toString()))
                                System.out.println(a.toString());
                        }   
                    }
            }
            // TODO: Code to do the spell checker. Look into the StringSet for all possible alternatives of the input word mis-spelled by one character.
	  }
			
      } catch (FileNotFoundException e) {
      System.out.println("Cannot open file " + f.getAbsolutePath());
      System.out.println(e);
    } 
  } 
}
