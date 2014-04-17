#ifndef KEYMANAGEMENT_H_
#define KEYMANAGEMENT_H_

class KeyManagement {
	public:
		// 
	private:
		// Idle time
		unsigned long idle_time;
		
		// Start time
		unsigned long start_time;
		
		// Key down single function
		void key_down_callback();
			
		// Key up functions
		// Up
		void up_key_callback();
		// Down
		void down_key_callback();
		// Select
		void select_key_callback();
						
		// Register all LCDKeypadDriver events
		void init();
				
};

#endif /* KEYMANAGEMENT_H_ */