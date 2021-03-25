#include <iostream>
#include "Teeny.h"
#include <string>
#include <conio.h>
#include <stdio.h>
#include <time.h>
using namespace std;

struct Node
{
	char ch;
	Node* next;
	Node* prev;
};

COORD cord;		//coordinates to keep track of cursor


//Skeleton of class LinkedText
class LinkedText{
	Node *head, * tail, * cursorPos;
    int textSize;

    public:
        LinkedText(){
            head=tail=cursorPos=NULL;
            textSize=0;
        }
        //some suggested methods
        //All these methods (except copying) are O(1) thanks to the linked list!
		void addCharAtCursorPos(char input)//when char entered
		{
			
			if (textSize == 0)
			{
				Node* dummyHead = new Node;		//Dummy node at head
				Node* dummyTail = new Node;		//Dummy node at tail
				head = dummyHead;
				tail = dummyTail;
				//head->ch = NULL;
				//tail->ch = NULL;
				
				Node* temp = new Node;		//Node for new Char
				temp->ch = input;
				
				//Change DummyHead pointers
				head->next = temp;
				head->prev = nullptr;
				//Change new Node pointers
				temp->prev = head;
				temp->next = tail;
				//Change DummyTail pointers
				tail->prev = temp;
				tail->next = nullptr;
				//change CursorPos pointer
				cursorPos = temp;

				textSize = 1;

				
			}
			else
			{
				//Insert At head
				if (cursorPos == head)
				{
					Node* temp = new Node;
					temp->ch = input;
					
					//Change new Node pointers
					temp->prev = head;
					temp->next = cursorPos->next;
					//Change DummyHead pointers
					head->next = temp;
					head->prev = nullptr;
					//change CursorPos pointer
					cursorPos = cursorPos->next;
					//Change successor Node prev pointers
					temp = temp->next;
					temp->prev = cursorPos;

					textSize++;

					
					
				}
				else if (cursorPos == tail)//Insert At Tail
				{
					Node* temp = new Node;
					temp->ch = input;
					
					//Change DummyTail pointers
					tail->prev = temp;
					tail->next = nullptr;
					//Change new Node pointers
					temp->prev = cursorPos->prev;
					temp->next = tail;
					//Change predecessor Node next pointers
					temp = temp->prev;
					temp->next = cursorPos;
					//change CursorPos pointer
					//cursorPos = cursorPos->next;
					
					textSize++;

					
				}
				else	//Insert in Middle
				{
					Node* temp = new Node;
					temp->ch = input;

					//Change new Node pointers
					temp->prev = cursorPos;
					temp->next = cursorPos->next;
					//Change predecessor Node next pointer
					cursorPos->next = temp;
					//change CursorPos pointer
					cursorPos = cursorPos->next;
					//Change successor Node prev pointer
					temp = temp->next;
					temp->prev = cursorPos;

					textSize++;

					
				}
				
			}
		}
		void delCharAtCursorPos()//when bksp pressed
		{
			if (textSize > 0)
			{
				if (cursorPos == head)	//Cursor is pointing to DummyHead
				{
					return;		//delete nothing
				}
				else if (cursorPos == head->next)	//Cursor is at First Node
				{
					//Change predecessor Node next pointer
					head->next = cursorPos->next;
					//Change successor Node prev pointer
					Node* temp = cursorPos->next;
					temp->prev = head;
					//Change Current Node pointers
					cursorPos->next = nullptr;
					cursorPos->prev = nullptr;

					//delete and move CursorPos to previous Node
					delete cursorPos;
					cursorPos = temp->prev;

					textSize--;
				}
				else	//Cursor is at Last Node or in Middle
				{
					if (cursorPos == tail)	//if cursor is at DummyTail
					{
						cursorPos = tail->prev;
					}
					//Change Previous and Next Node pointers
					Node* t1 = cursorPos->prev;	//Previous Node ptr
					Node* t2 = cursorPos->next;	//Next Node ptr

					t1->next = t2;
					t2->prev = t1;
					//Change Current Node pointers
					cursorPos->prev = cursorPos->next = nullptr;
					//Delete and move cursor to previous Node
					delete cursorPos;
					cursorPos = t1;

					textSize--;
				}

			}


		}
		void moveCursorRight()//right arrow
		{
			if(cursorPos->next!=nullptr)
				cursorPos = cursorPos->next;
		}
		void moveCursorLeft()//left arrow
		{
			if (cursorPos->prev != nullptr)
				cursorPos = cursorPos->prev;
		}
        /*the following method returns the sub-list after cutting,
        and also adjusts cursor position*/
		Node* cutTextBetween(Node* cursorpos1, Node* cursorpos2, bool reverse)
		{
			Node* temp1 = cursorpos1->prev;
			Node* temp2 = cursorpos2->next;

			temp1->next = temp2;
			cursorpos1->prev = nullptr;

			temp2->prev = temp1;
			cursorpos2->next = nullptr;

			//Update textSize
			Node* count = cursorpos1;
			int x = 0;
			while (count != cursorpos2)
			{
				x++;
				count = count->next;
			}
			x++;
			textSize = textSize - x;
			if (cord.X != 0 && reverse == false)
				cord.X -= x;
			else if (cord.X != 0 && reverse == true)
				cord.X--;
			//Change Cursorpos
			cursorPos = temp1;

			Node* Temp = cursorpos1;	//return a sub-list of Nodes pointed by Temp
			return Temp;
		}
        /*the following method returns the duplicated sub-list after copying,
        and also adjusts cursor position*/
		Node* copyTextBetween(Node* cursorpos1, Node* cursorpos2)
		{
			//pos1 is starting point and pos2 is ending point
			Node* copy = cursorpos1;
			Node* copy_prev = nullptr;
			Node* copy_next = nullptr;
			Node* copy_head = nullptr;

			if (cursorpos1 == cursorpos2)
			{
				//Only 1 Node should be copied
				copy_head = new Node;
				copy_head->ch = copy->ch;
				copy_head->prev = copy_head->next = nullptr;
			}
			else
			{

				while (copy != cursorpos2->next)	//while !endpoint, make copies of nodes
				{
					if (copy == cursorpos1)
					{
						copy_prev = new Node;
						copy_prev->ch = copy->ch;
						copy_prev->prev = nullptr;
						copy_head = copy_prev;
					}
					else
					{
						copy_next = new Node;
						copy_next->ch = copy->ch;
						copy_next->prev = copy_prev;
						copy_prev->next = copy_next;
						copy_prev = copy_prev->next;
						copy_next->next = nullptr;
					}
					copy = copy->next;
				}
				
			}

			return copy_head;
		}
		void insertTextAtCursorPos(Node* subList)
		{
			int x = 0;	//for counting how many nodes will be added
			if (cursorPos == head)
			{
				Node* last = subList;
				while (last->next != nullptr)	//find last node pointer for the sublist
				{
					x++;
					last = last->next;
				}
				x++;
				textSize = textSize + x;

				Node* temp = cursorPos->next;	//pointer pointing to continuing list

				//Change pointers
				head->next = subList;
				subList->prev = head;

				last->next = temp;
				temp->prev = last;

				//Change Cursorpos
				cursorPos = last;
			}
			else if (cursorPos == tail)
			{
				Node* last = subList;
				while (last->next != nullptr)	//find last node pointer for the sublist
				{
					x++;
					last = last->next;
				}
				x++;
				textSize = textSize + x;

				Node* temp = cursorPos->prev;	//pointer pointing to continuing list

				//Change pointers
				temp->next = subList;
				subList->prev = temp;

				last->next = tail;
				tail->prev = last;

				//Change Cursorpos
				cursorPos = last;
			}
			else
			{
				Node* last = subList;
				while (last->next != nullptr)	//find last node pointer for the sublist
				{
					x++;
					last = last->next;
				}
				x++;
				textSize = textSize + x;

				Node* temp = cursorPos->next;	//pointer pointing to continuing list

				//Change pointers
				cursorPos->next = subList;
				subList->prev = cursorPos;

				last->next = temp;
				temp->prev = last;

				//Change Cursorpos
				cursorPos = last;
			}

			//Change cord pos on screen
			cord.X += x;
			
		}

        /*Method returns the number of times key occurs
        in LinkedText*/
		int findInText(const string& key)
		{
			int count, i;
			i = count = 0;
			
			for (Node* search = head->next;search != tail;search = search->next)
			{
				if (search->ch == key[i])
				{
					i++;
					if (key[i] == '\0')		//if found, then look for complete word
					{
						i = 0;
						count++;
					}
				}
				else		//If not found then, reset string to first char
				{
					i = 0;
				}
				
			}
			return count;
		}

        //You may add appropriate printing methods and other methods below
        ~LinkedText()  //delete the list
		{
			if (cursorPos != nullptr)
			{
				Node* current = head;
				Node* next;

				while (current != NULL)
				{
					next = current->next;
					delete current;
					current = next;
				}
				head = tail = cursorPos = nullptr;
			}
        }

		int returnSize()const
		{
			return textSize;
		}
		Node* GetCursorPos()
		{
			return cursorPos;
		}

		Node* GetHead()
		{
			return head;
		}

		void print()const
		{
			if (textSize > 0)
			{
				Node* temp = head->next;
				while (temp != tail)
				{
					cout << temp->ch;
					temp = temp->next;
				}
			}
		}

        /*The methods below will be used to keep
          the screen cursor inside the text area
          only.
        */
        inline bool textToLeft(){
            return (cursorPos!=head);
        }


        inline bool textToRight(){
            return (cursorPos->next!=tail);
        }

};

int main()
{
    GetConsoleWindowDims();
    /*This starter code is capable of detecting the
      left and right arrow keys; the backspace key;
      the escape key (for quit); the F1, F2 and F3 keys;
      and the regular text keys.
      Written for CS-218 "Data Structures" BCS-3A, FAST-NU, Lahore.
    */
	
    LinkedText text;
    int key = 0;
	//Utility variables
	int F1count = 0;
	bool ReversedSelection = false;
	//pointers for selecting text
	Node* start;
	Node* end;
	Node* subList;
	start = end = subList = nullptr;
	//coordinates for cursor
	cord.X = 0, cord.Y = 0;
    while(key!=ESCAPE){
          key=_getch();

		  // Changing the color of the Text
		  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		  int range = (15 - 1) + 1;
		  int color = rand() % range + 1;
		  SetConsoleTextAttribute(hConsole, color);

		  //either an arrow or function key is pressed
          if(SPECIAL_KEY(key)){
              key=_getch();//get 2nd part of code from buffer
              switch(key){
              case ARROW_LEFT:
                  if(text.textToLeft())
				  {
                    //moveScreenCursorLeft();
					cord.X--;
					gotoxy(cord.X, cord.Y);
                    text.moveCursorLeft();
                  }
                break;
              case ARROW_RIGHT:
                  if (text.textToRight())
				  {
                    //moveScreenCursorRight();
					cord.X++;
					gotoxy(cord.X, cord.Y);
					text.moveCursorRight();
                  }
                break;
              case F1://start or end text selection
			  {
				  if (F1count == 0)
				  {
					  //remember starting Node pos

					  if (text.textToLeft() == false)	//Avoid selecting the Dummy Node at Head
					  {
						  start = text.GetCursorPos()->next;
						  F1count = 1;
					  }
					  else
					  {
						  start = text.GetCursorPos();
						  F1count = 1;
					  }
				  }
				  else if (F1count == 1)
				  {
					  //remember Ending Node pos
					  if (text.textToLeft() == false)	//Avoid selecting the Dummy Node at Head
					  {
						  end = text.GetCursorPos()->next;
						  F1count = 0;
					  }
					  else
					  {
						  end = text.GetCursorPos();
						  F1count = 0;
					  }
					  //Check for Reverse Selection
					  Node* check = text.GetHead();
					  while (check != nullptr)
					  {
						  if (check == start)
						  {
							  break;
						  }
						  else if(check==end)
						  {
							  //swap start and end
							  Node* temp;
							  temp = start;
							  start = end;
							  end = temp;
							  ReversedSelection = true;
							  break;
						  }
						  check = check->next;
					  }
				  }
			  }
                break;
              case F2://cut option
			  {
				  if (start != nullptr && end != nullptr)
				  {
					  subList = text.cutTextBetween(start, end, ReversedSelection);
					  system("cls");
					  text.print();
					  //update cursor on screen
					  gotoxy(cord.X, cord.Y);
				  }
			  }
                break;
              case F3://copy option
			  {
				  if (start != nullptr && end != nullptr)
					subList = text.copyTextBetween(start, end);
			  }
                break;
              case F5://for search
			  {
				  string word;
				  cout << "\nEnter the Word to search: ";
				  getline(cin, word);
				  cout << "\"" << word << "\" occurs in the Text " << text.findInText(word) << " times!"<<endl;
				  cout << "Press ENTER to get back to editor..." << endl;
				  key = _getch();
				  if (key == ENTER)
				  {
					  system("cls");
					  text.print();
					  gotoxy(cord.X, cord.Y);
				  }
			  }
                break;
              case INSERT://to insert text
			  {
				  if (subList != nullptr)
				  {
					  text.insertTextAtCursorPos(subList);
					  system("cls");
					  text.print();

					  //update cursor on screen
					  gotoxy(cord.X, cord.Y);
					  //Make pointers NULL to avoid multiple insertions
					  subList = nullptr;
					  start = end = nullptr;
				  }
			  }
                break;
              };
          }else if(key==BACKSPACE)
		  {
			  if (text.textToLeft())
			  {
				 system("cls");
				 text.delCharAtCursorPos();
				 cord.X--;
				 text.print();
				 gotoxy(cord.X,cord.Y);
			  }
          }else
		  {
			 
			  system ("cls");
			  text.addCharAtCursorPos(key);
			  
			  text.print();
			  
			  cord.X++;
			  gotoxy(cord.X, cord.Y);
			  

			  //Forget previous selection, if start typing between selection
			  if (F1count == 1)
			  {
				  F1count = 0;
			  }
			 
          }
    }
	
    return 0;
}
