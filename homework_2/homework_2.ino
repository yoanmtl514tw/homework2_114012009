const int RledPin =9;
const int GledPin =10;
const int BledPin =11;
const int buttonPin = 2;


int mood =0;
const int neutral_mood =10;
int button_state =0;
bool button_pressed = false;
unsigned long touched_timer = 0;
unsigned long reduced_timer = 0;
const long untouch_interval = 5000;
const long reduced_interval = 1000;


unsigned long previous_millis =0;
int fading_direction =1;

unsigned long blink_timer = 0;
bool led_visible = true;
const long blink_interval = 200;


void setup()
{

  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  //Initial mood = neutral
  mood = neutral_mood;

}

void loop()
{

  //show_led_state(mood);

  button_state = digitalRead(buttonPin);

  if(button_state == HIGH && !button_pressed)
  {
    mood = max_mood(mood + 1);
    if(mood > 20) mood =20;
    touched_timer = millis();
    button_pressed = true;
  }

  if(button_state == LOW && button_pressed)
  {
    button_pressed = false;
  }

  //toy getting sad 
  unsigned long current_timer = millis();
  if(current_timer - touched_timer > untouch_interval)
  {
    if(current_timer - reduced_timer > reduced_interval)
    {
      mood = max_mood(mood-1);
      reduced_timer = current_timer;
    }
  }

  //blinking
  update_blink_state();

  if(led_visible || mood >= neutral_mood)
  {
    show_led_state(mood);
  }
  else
  {
    analogWrite(RledPin, 255);
    analogWrite(GledPin, 255);
    analogWrite(BledPin, 255);
  }

  show_mood(mood);

}

//FUNCTION 


void show_led_state(int state)
{

 float brightness_interval = 255 / 10.0;
 if(mood >= neutral_mood)
 {
  analogWrite(RledPin, 255);
  analogWrite(GledPin, brightness_interval * (mood - neutral_mood));
  analogWrite(BledPin, 255 - brightness_interval *(mood - neutral_mood));
 }

 else
 {
  analogWrite(RledPin, 255 - brightness_interval *(neutral_mood - mood));
  analogWrite(GledPin, brightness_interval * (neutral_mood - mood));
  analogWrite(BledPin, 255);
 }

}

//Function to limitate the mood between 0 and 20
int max_mood(int mood)
{
  if (mood < 0)
  {
    mood =  0;
  }
  if (mood > 20)
  {
    mood = 20;
  }

  return mood;
}

//Funcion to blink the led when mood is under < neutral mood
void update_blink_state()
{
  if(mood < 4 && mood > 0)
  {
    unsigned long time = millis();
    if(time - blink_timer > blink_interval)
    {
      led_visible = !led_visible;
      blink_timer = time;
    }
  }
  else
  {
    led_visible = true;
  }
}

//Fuction to display the mood in the display monitor

void show_mood(int mood)
{
  //If happy
  if(mood >=11 && mood <=20)
  {
    Serial.print("Mood: ");
    //Serial.print(mood);
    Serial.println(" 😊 Happy");
  }

  //if neutral
  else if( mood >= 2 and mood <=10)
  {
    Serial.print("Mood: ");
    //Serial.print(mood);
    Serial.println(" 😐 Neutral");
  }

  //if sad
  else if(mood <= 1)
  {
    Serial.print("Mood: ");
    //Serial.print(mood);
    Serial.println(" 😢 Sad");
  }
}

