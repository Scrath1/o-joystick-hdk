union Btn_Meta_t{
	struct __attribute__ ((packed)){
		int16_t thumbstick1_val;
		int16_t thumbstick2_val;
		int16_t topHat1_val;
		int16_t topHat2_val;
		int16_t thumbHat1_val;
		int16_t thumbHat2_val;
		int16_t triggerButtons_val;
		int8_t brakeButton_val;
		int8_t topSideButton_val;
		int8_t pinkyButton_val;
	} data;
	
	char rawData[sizeof(data)];
};

void printBtn_Meta_t(Btn_Meta_t stru){
  Serial.print("Thumbstick1:");
  Serial.println(stru.data.thumbstick1_val);
  Serial.print("Thumbstick2:");
  Serial.println(stru.data.thumbstick2_val);
  Serial.print("topHat1:");
  Serial.println(stru.data.topHat1_val);
  Serial.print("topHat2:");
  Serial.println(stru.data.topHat2_val);
  Serial.print("thumbHat1:");
  Serial.println(stru.data.thumbHat1_val);
  Serial.print("thumbHat2:");
  Serial.println(stru.data.thumbHat2_val);
  Serial.print("triggerButtons:");
  Serial.println(stru.data.triggerButtons_val);
  Serial.print("brakeButton:");
  Serial.println(stru.data.brakeButton_val);
  Serial.print("topSideButton:");
  Serial.println(stru.data.topSideButton_val);
  Serial.print("pinkyButton:");
  Serial.println(stru.data.pinkyButton_val);
  }
