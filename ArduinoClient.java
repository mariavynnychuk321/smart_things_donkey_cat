package arduino_client;

import java.util.Scanner;
import jssc.SerialPort;
import jssc.SerialPortException;

public class ArduinoClient {
 
	public static void main(String[] args) throws InterruptedException {
 
		//Define Port
		SerialPort serialPort = new SerialPort("COM5");
 
		try {
			//Open Serial Port and set Parameters
			serialPort.openPort();
			serialPort.setParams(9600,8,1,0);
			System.out.println("port opened");
			Thread.sleep(4000);
 
			String data = "9:40 10:40 11:40 9:40 10:40 11:40";
			serialPort.writeString(data);
			System.out.println("data sent");
			
			serialPort.closePort();
			System.out.println("port closed");
			 
		}
		catch (SerialPortException ex) {
			System.out.println(ex);
		}
	}
 
}
