
#define WhiteBread 99 
#define Lettuce 44 
#define Turkey 1 
#define Chicken 3 
#define Salami 4 
#define Cheese 2 
int bread;
int main_ingredient;

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  Serial.setTimeout(7000);
  Serial.println("Sandwich Order:");
  Serial.println("Choose your bread:");
  Serial.print("White Bread: ");
  Serial.println(WhiteBread);
  Serial.print("Gluten Free(Lettuce): ");
  Serial.println(Lettuce);
  bread = Serial.parseInt();
  delay(200);

    Serial.println("Choose your main ingredient:");
    Serial.print("Turkey: ");
    Serial.println(Turkey);
    Serial.print("Chicken: ");
    Serial.println(Chicken);
    Serial.print("Salami: ");
    Serial.println(Salami);
    Serial.print("Cheese: ");
    Serial.println(Cheese);
    main_ingredient = Serial.parseInt();
    delay(200);

    if( (bread == WhiteBread || bread == Lettuce) && (main_ingredient == Turkey || main_ingredient == Chicken || main_ingredient == Salami || main_ingredient == Cheese) ) {
      Serial.println("Your Order:");
      Serial.print("Bread: ");
      Serial.println(bread);
      Serial.print("Main Ingredient: ");
      Serial.println(main_ingredient);
      Serial.print("bread: ");
      Serial.println(bread);
      main_ingredient = 0;
      bread = 0;
      delay(5000);
    } else {
    Serial.println("Invalid selection.");
    }



}
