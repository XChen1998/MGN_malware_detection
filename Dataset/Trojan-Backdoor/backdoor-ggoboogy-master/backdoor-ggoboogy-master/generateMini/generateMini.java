// ** WARNING **
// This is written by ggoboogy (Ji Hyeon Yoon, KAIST)
// If you want to use this code, PLEASE FORK THIS REPOSITORY.

import java.io.*;
import java.util.*;

public class generateMini
{
    static int memEnd = 0;
    static Hashtable<String, Integer> str_location = new Hashtable<String, Integer>(); // It will contain string location for reusing
    static Hashtable<Integer, Integer> line_map = new Hashtable<Integer, Integer>(); // It will contain mapping between input line number and output line num.
    static ArrayList <String> result = new ArrayList <String> ();

    static int input_line_num = 0;
    
    public static void main(String [] argv) throws Exception 
    {
        File file = new File(argv[0]);
        FileReader fileReader = new FileReader(file);
        BufferedReader reader = new BufferedReader(fileReader);
        String line = "";

        while((line = reader.readLine()) != null)
        {
            ++input_line_num;
            String [] parse = line.split("\\(");
            
            if(parse.length > 1)
                parse[1] = parse[1].replaceAll("\\)","");

            if(parse[0].compareTo("puts") == 0) 
            {
                line_map.put(input_line_num, result.size()); 
           //     System.out.println("***"+ input_line_num + " : " + (result.size()+1));

                parse[1] = parse[1].replaceAll("\"","");
                
                if(!str_location.containsKey(parse[1])) // if String is not existing
                    StoreString(parse[1]);
                 
                result.add("puti r0, 0x" + Integer.toHexString(str_location.get(parse[1])));
                result.add("puts r0");
            }
            else if(parse[0].compareTo("gets") == 0)
            {
                line_map.put(input_line_num, result.size());
         //       System.out.println("***"+ input_line_num + " : " + (result.size()+1));

                result.add("puti r0, 0x" + Integer.toHexString(memEnd));
                result.add("gets r0");
                
                str_location.put(parse[1], memEnd);
                memEnd += 50; // I can't expect how many lines I will got, So I made random decision.
            }
            else if(parse[0].compareTo("strcmp") == 0)
            {
                line_map.put(input_line_num, result.size());
             //   System.out.println("***"+ input_line_num + " : " + (result.size()+1));

                String [] param = parse[1].split(",");
                param[0] = param[0].replaceAll("\"","");
                param[1] = param[1].replaceAll("\"","");
                int param0_start = 0;
                int param1_start = 0;
                
                if(str_location.get(param[0]) == null)   
                    StoreString(param[0]);
                
                if(str_location.get(param[1]) == null)
                    StoreString(param[1]);

                param0_start = str_location.get(param[0]);
                param1_start = str_location.get(param[1]);

               // initialize
                result.add("puti r0, 0x" + Integer.toHexString(param0_start));
                result.add("puti r1, 0x" + Integer.toHexString(param1_start));
                result.add("puti r6, 0x1"); //to add 1byte
                result.add("puti r7, 0x0"); // to compare zero or not

               // loop
                int loop_start = result.size();

                result.add("load r2, r0"); // 0
                result.add("load r3, r1"); // 1
                result.add("add r0, r0, r6"); // 2: reload
                result.add("add r1, r1, r6"); // 3: reload
                
                result.add("add r12, r2, r3"); // 4
                result.add("eq r13, r7, r12"); // 5
                result.add("ite r13, *" + param[2] + ", 0x" + Integer.toHexString(loop_start+7)); // 6

                result.add("eq r10, r7, r2"); // 7
                result.add("ite r10, *" + param[3] + ", 0x" + Integer.toHexString(loop_start+9)); // 8

                result.add("eq r11, r7, r3"); // 9
                result.add("ite r11, *" + param[3] + ", 0x" + Integer.toHexString(loop_start+11)); // 10

                result.add("eq r13, r2, r3"); // 11
                result.add("ite r13, 0x" + Integer.toHexString(loop_start+13) + ", *" + param[3]); //12

                result.add("jump 0x" + Integer.toHexString(loop_start)); // 13
            }
            else if(parse[0].compareTo("exit") == 0)
            {
            //    System.out.println("***"+ input_line_num + " : " + (result.size()+1));
                line_map.put(input_line_num, result.size());
                result.add("halt");
            }
        }
        for(int j=0; j<result.size(); j++)
        {
            String imsi = result.get(j);
            int idx = 0;
          
            if((idx = imsi.indexOf("*")) != -1)
            {
                String tmp = imsi.substring(idx+1);
                String [] num = tmp.split(",");
                String tmp1 = "*" + num[0];
                String tmp2 = "0x" + Integer.toHexString(line_map.get(Integer.parseInt(num[0])));

                imsi = imsi.replace(tmp1, tmp2);
                result.set(j, imsi);
            }
            System.out.println(result.get(j));
        } 
        reader.close();  
    }
    public static void StoreString(String str)
    {
        result.add("puti r0, 0x" + Integer.toHexString(memEnd));
        result.add("puti r1, 0x1");
                
        for(int i=0; i<str.length(); i++)
        {
            String imsi = str.substring(i,i+1);
            result.add("puti r2, " + stringToHex0x(imsi));
            result.add("store r0, r2");
            result.add("add r0, r0, r1");
        }
        result.add("puti r2, 0x0"); //add null at the end of string
        result.add("store r0, r2");

        str_location.put(str, memEnd); 
        memEnd += (str.length() + 1);
    }

    public static String stringToHex0x(String s)
    {
        String result = "";
        
        for(int i=0; i<s.length(); i++)
        {
            result += String.format("0x%02X ", (int) s.charAt(i));
        }
        return result;
    }
}
 
