float getSonar()
{
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  uint32_t pingTime = pulseIn(ECHOPIN, HIGH, (MAX_DISTANCE * 60));
  return (float)pingTime * 340.0f / 2.0f / 10000.0f;
}
