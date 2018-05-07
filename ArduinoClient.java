package catFeeder_UI;

import jssc.SerialPort;
import jssc.SerialPortException;

public class ArduinoClient {
 
	public static void sendData(String data) throws InterruptedException {
 
		SerialPort serialPort = new SerialPort("COM5");
 
		try {
			serialPort.openPort();
			serialPort.setParams(9600,8,1,0);
			Thread.sleep(4000);
			
			serialPort.writeString(data);	
			serialPort.closePort();
			 
		}
		catch (SerialPortException ex) {
			System.out.println(ex);
		}
		
	}
 
}
