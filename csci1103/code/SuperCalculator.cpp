#include <iostream>
using std::cout, std::endl, std::cin;
using std::string, std::to_string, std::stoi;
#include <cassert>

template <class E>
class ArrayQueue {
	private:
	E* storage;
	int storage_length;
	int m_count;
	int front;
	int back;
	
	void make_space() {
		E* storage2 = new E[storage_length + 1];
		for (int i=0; i < storage_length; i++) {
			int index = (front + i) % storage_length;
			assert(0 <= index && index < storage_length);
			storage2[i] = storage[index];
		}
		front = 0;
		back = storage_length - 1;
		delete [] storage;
		storage = storage2;
		storage_length++;
	}

	public:
	ArrayQueue() {
		storage = new E[5];
		storage_length = 5;
		m_count = 0;
		front = 3;
		back = 2;
	}
	
	~ArrayQueue() {
		delete [] storage;
	}

	// Disabling copying
	void operator=(ArrayQueue& s) {
		cout << "Error! Copying ArrayQueue is not implemented." << endl;
		assert(false);
	}
	ArrayQueue(ArrayQueue& s) {
		cout << "Error! Copying ArrayQueue is not implemented." << endl;
		assert(false);
	}
	
	int count() {
		return m_count;
	}

	bool is_empty() {
		return (count() == 0);
	}
	
	E dequeue() {
		assert(count() > 0);
		int old_front = front;
		front = (front + 1) % storage_length;
		assert(0 <= old_front && old_front < storage_length);
		m_count--;
		return storage[old_front];
	}
	
    E peek() {
        return storage[front];
    }

	void enqueue(E x) {
		if (count() == storage_length) {
			make_space();
		}
		back = (back + 1) % storage_length;
		assert(0 <= back && back < storage_length);
		m_count++;
		storage[back] = x;
	}
	
	void output() {
		cout << "[ ";
		for (int i=0; i < storage_length; i++) {
			if (front <= back) {
				if (front <= i && i <= back) {
					cout << storage[i] << " ";
				}
				else {
					cout << "- ";
				}
			}
			else {
				if (back < i && i < front) {
					cout << "- ";
				}
				else {
					cout << storage[i] << " ";
				}
			}
		}
		cout << "]" << endl;
	}

};

template <class E>
struct Node {
	E value;
	Node<E>* next;
	Node(E v, Node* n) {
		value = v;
		next = n;
	}
};

template <class E>
class LinkedStack {
	private:
	Node<E>* head;
	int m_count;

	public:
	LinkedStack() {
		head = nullptr;
		m_count = 0;
	}

	~LinkedStack() {
		Node<E>* current = head;
		while (current != nullptr) {
			Node<E>* todelete = current;
			current = current->next;
			delete todelete;
		}
	}

	// Disabling copying
	void operator=(LinkedStack& s) {
		cout << "Error! Copying LinkedStack is not implemented." << endl;
		assert(false);
	}
	LinkedStack(LinkedStack& s) {
		cout << "Error! Copying LinkedStack is not implemented." << endl;
		assert(false);
	}

	void push(E x) {
		Node<E>* old_head = head;
		head = new Node<E>(x, old_head);
		m_count++;
	}

	E pop() {
		Node<E>* old_head = head;
		assert(head != nullptr);
		head = head->next;
		E ret = old_head->value;
		delete old_head;
		m_count--;
		return ret;
	}

    E peek() {
        return head->value;
    }

	int count() {
		return m_count;
	}

	bool is_empty() {
		return (head == nullptr);
	}
};

int rank(string operation) {
    /* modified code from previous assignment.
       kept / so that this calculator could be expanded if desired */

    if (operation == "^") {
        return 2;
    }
    else if (operation == "/" || operation == "*") {
        return 1;
    }
    else if (operation == "+" || operation == "-") {
        return 0;
    }

    return -1;
}

string convert_to_postfix(ArrayQueue<string>& infix, ArrayQueue<string>& postfix) {// converts infix to postfix and returns postfix as string
    LinkedStack<string> operator_stack;
    string postfix_as_string = "";
    while (!infix.is_empty()) {
        if (rank(infix.peek()) == -1) {
            postfix_as_string = postfix_as_string + infix.peek() + " ";
            postfix.enqueue(infix.dequeue());
        }
        else if (operator_stack.is_empty()) {
            operator_stack.push(infix.dequeue());
        }
        else {
            while (!operator_stack.is_empty()) {
                if (rank(infix.peek()) == 2) {
                    break;
                }
                if (rank(operator_stack.peek()) >= rank(infix.peek())) {
                    postfix_as_string = postfix_as_string + operator_stack.peek() + " ";
                    postfix.enqueue(operator_stack.pop());
                }
                else {
                    break;
                }
            }
            operator_stack.push(infix.dequeue());
        }
    }

    while (!operator_stack.is_empty()) {
        postfix_as_string = postfix_as_string + operator_stack.peek() + " ";
        postfix.enqueue(operator_stack.pop());
    }

    return postfix_as_string;
}

void left_align(string& num1, string& num2) {
    if (num1.length() > num2.length()) {
        while (num1.length() != num2.length()) {
            num2 = "0" + num2;
        }
    }
    else if (num2.length() > num1.length()) {
        while (num2.length() != num1.length()) {
            num1 = "0" + num1;
        }
    }
}

string add(string num1, string num2) {
    left_align(num1, num2);
    int length = num1.length();
    
    string sum = "";
    int carry = 0;

    for (int i = length - 1; i >= 0; i--) {
        int digit1 = num1[i] - '0'; 
        int digit2 = num2[i] - '0';
        string digit_sum;
        
        if (i == 0) {
        digit_sum = to_string(digit1 + digit2 + carry);
        }
        else {
        digit_sum = to_string((digit1 + digit2 + carry) % 10);
        carry = (digit1 + digit2 + carry) / 10;
        }

        sum = digit_sum + sum;
    }

    return sum;
}

string multiply(string num1, string num2) {
    if (num1 == "0" || num2 == "0") {
        return "0";
    }

    int digits_in_smaller;
    string smaller;

    int digits_in_larger;
    string larger;

    string total = "0"; 

    if (num1.length() < num2.length()) { 
    // ensures that traditional multiplication rules are followed (smaller number goes below larger number)
        smaller = num1;
        larger = num2;
    }
    else {
        smaller = num2;
        larger = num1;
    }

    digits_in_smaller = smaller.length();
    digits_in_larger = larger.length();

    int zeros_to_append = 0;
    for (int i = digits_in_smaller - 1; i >= 0; i--) {
        int carry = 0;
        string to_add = "";
        int digit_smaller = smaller[i] - '0'; 
        for (int j = digits_in_larger - 1; j >= 0; j--) {
            string digit_product;
            int digit_larger = larger[j] - '0';
            if (j == 0) {
                digit_product = to_string(digit_smaller * digit_larger + carry);
            }
            else {
                digit_product = to_string((digit_smaller * digit_larger + carry) % 10);
                carry = (digit_smaller * digit_larger + carry) / 10;
            }
            to_add = digit_product + to_add;
        }        

        for (int k = 0; k < zeros_to_append; k++) {
            to_add = to_add + "0";
        }
        total = add(total, to_add);
        zeros_to_append++; 
    } 
    return total;
}

string subtract(string num1, string num2) {
    if ((num1.length() < num2.length())) {
        return "-1";
    }
    
    if (num1.length() == num2.length() && num1 < num2) {
        return "-1";
    }

    if (num1 == num2) {
        return "0";
    }

    left_align(num1, num2);
    int length = num1.length(); 

    string difference = "";
    for (int i = length - 1; i >= 0; i--) {
        int num1_digit = num1[i] - '0';
        int num2_digit = num2[i] - '0';

        if (num1_digit < num2_digit) { // borrowing
            int borrow_from = num1[i - 1] - '0';
            borrow_from--;
            num1_digit += 10;
            num1[i - 1] = borrow_from + '0';
        }
        
        string digit_difference = to_string(num1_digit - num2_digit);
        if (i == 0 && (digit_difference == "0")) {break;}

        difference = digit_difference + difference;
    }
    
    return difference;
}

string exponent(string base, string power) {
    int int_power = stoi(power);
    string result = "1";
    for (int i = 1; i <= int_power; i++) {
        result = multiply(result, base);
    }
    
    return result;
}

string calculate(ArrayQueue<string>& postfix) {
    LinkedStack<string> number_stack;
    string num1;
    string num2;
    while (!postfix.is_empty()) {
        if (rank(postfix.peek()) == -1) {
            number_stack.push(postfix.dequeue());
            continue;
        }

        num2 = number_stack.pop();
        num1 = number_stack.pop();
        if (postfix.peek() == "^") {
            postfix.dequeue();
            number_stack.push(exponent(num1, num2));
        }
        else if (postfix.peek() == "*") {
            postfix.dequeue();
            number_stack.push(multiply(num1, num2));
        }
        else if (postfix.peek() == "+") {
            postfix.dequeue();
            number_stack.push(add(num1, num2));
        }
        else if (postfix.peek() == "-") {
            postfix.dequeue();
            string difference = subtract(num1, num2);
            if (difference == "-1") {
                return "Error, negative encountered";
            }
            number_stack.push(difference);
        }
    }

    return number_stack.pop();
}

int main() {
    ArrayQueue<string> infix;
    string token;

    cin >> token;
    while (token != "=") {
        infix.enqueue(token);
        cin >> token;
    }
    
    ArrayQueue<string> postfix;
    string postfix_as_string = convert_to_postfix(infix, postfix);
    cout << "Postfix: " << postfix_as_string << endl;
    
    string result = calculate(postfix);
    cout << "Result: " << result << endl;

    return 0;
}

