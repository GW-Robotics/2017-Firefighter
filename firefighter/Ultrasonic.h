namespace nanpy {

	class Ultrasonic {
		
		private:
			int echo, trig, conversionFactor;
			
		public:
			Ultrasonic(int echoPin, int trigPin, bool useInches);
			float getDistance();
	};
}
