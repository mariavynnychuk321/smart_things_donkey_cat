package catFeeder_UI;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;

public class UI extends Application {
	
	int counter = 0;	// how much fields we have 
	
	@Override
	public void start(Stage primaryStage) {
	    
		GridPane root = new GridPane();
		
	    root.setHgap(10);
	    root.setVgap(10);
	    
	    Label label1 = new Label("Вкажіть час спрацьовування");
	    root.add(label1, 5, 3); 
	   
	    // for data input
	    TextField textField[] = new TextField[20];
	    
	    // =========== first "init" fields ==============
	    // hour
        textField[counter] = new TextField();
        textField[counter].setPrefWidth(50);
        root.add(textField[counter], 8, counter + 3); 
        
        Label d = new Label(":");
        root.add(d, 9, counter + 3);
        
        // minute
        textField[counter + 1] = new TextField();
        textField[counter + 1].setPrefWidth(50);
        root.add(textField[counter + 1], 10, counter + 3);
        
        counter += 2;
        
	    // =========== text inputs adder ===========
	    Button fieldsAdder = new Button("додати час");
	    root.add(fieldsAdder, 5, 5);
	    
	    fieldsAdder.setOnAction(e -> {
	    	
	    	// hour
	        textField[counter] = new TextField();
	        textField[counter].setPrefWidth(50);
	        root.add(textField[counter], 8, counter + 3); 
	        
	        Label d1 = new Label(":");
	        root.add(d1, 9, counter + 3);
	        
	        // minute
	        textField[counter + 1] = new TextField();
	        textField[counter + 1].setPrefWidth(50);
	        root.add(textField[counter + 1], 10, counter + 3);
	        
	        counter += 2;
	        
	    });
	    
	    // =========== data sender ===========
	    Button makeData = new Button("вивантажити");
	    root.add(makeData, 5, 7);
	    
	    makeData.setOnAction(e -> {
	    	
	    	String data = "";	// what we will actually send
	    	
	    	// collect data from inputs
	    	for (int i = 0; i < counter; i++) {
	    		
	    		// to avoid space in the end of string for ok parsing
	    		if (i == counter - 1) {
	    			data = data + textField[i].getText();
	    		}
	    		else { 
	    			data = data + textField[i].getText() + " ";
	    		}
	    		
	    	}
	    	
	    	//System.out.println(data);
	    	
	    	try {
	    		ArduinoClient.sendData(data);
			} 
	    	catch (InterruptedException ie) {
				System.out.println(ie);
			}
	    	
	    });
	
	    final Scene scene = new Scene(root, 500, 300);
	    primaryStage.setScene(scene);
	    primaryStage.show();
	
	}
	
	public static void main(String[] args) {
	    launch(args);
	}
	
}
