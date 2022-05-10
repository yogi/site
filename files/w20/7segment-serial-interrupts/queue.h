#define MAX 8

char arr[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

bool queue_isEmpty() {
   return itemCount == 0;
}

bool queue_isFull() {
   return itemCount == MAX;
}

int queue_size() {
   return itemCount;
}  

void queue_enqueue(char data) {

   if(!queue_isFull()){
	
      if(rear == MAX-1){
         rear = -1;            
      }       

      arr[++rear] = data;
      itemCount++;
   }
}

char queue_dequeue() {
   char data = arr[front++];
	
   if(front == MAX){
      front = 0;
   }
	
   itemCount--;
   return data;  
}