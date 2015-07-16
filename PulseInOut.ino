void ThruPulse()
{
  pulse_ch1 = pulseIn(RC_CH1, HIGH, 25000); // Read the pulse width of  
  pulse_ch2 = pulseIn(RC_CH2, HIGH, 25000); // each channel
  
  if(pulse_ch1<1000||pulse_ch1>2000){pulse_ch1=1500;}
  if(pulse_ch2<1000||pulse_ch2>2000){pulse_ch2=1500;}
  MTR_R.writeMicroseconds(pulse_ch1);
  MTR_L.writeMicroseconds(pulse_ch2);
  delay(5);

  Serial.print("motor right:");
  Serial.print(pulse_ch1);
  Serial.print(", motor left:");
  Serial.println(pulse_ch2);
}
