#include <windows.h>
#include "resource.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char g_szClassName[] = "WindowsApp";
HINSTANCE hInstance;

FILE *f = fopen("dictionar.txt", "r");
struct node
{
	char word[20];
	char meaning[20];
	struct node *left, *right;
};

node *tree_root;

struct node* newNode(char *w, char *m)
{
	struct node* node = (struct node*)malloc(sizeof(struct node));

	strcpy(node->word, w);
	strcpy(node->meaning, m);

	node->left = node->right = NULL;
	return (node);
}

void separateword(char *str, char *w, char *m)
{
	int i, j, pos = -1;

	for (i = 0; str[i] != ' '; i++)
	{
		pos++;
		w[i] = str[i];
	}

	w[i++] = NULL;

	int k = -1;

	for (j = pos + 2; j<strlen(str) - 1; j++)
	{
		k++;
		m[k] = str[j];
	}
	m[k + 1] = NULL;
}

void addword(node *tree, char *word, char *meaning)
{

	node *p, *q;
	p = q = tree;

	while (p != NULL)
	{
		q = p;
		if (strcmp(word, p->word)<0)
			p = p->left;
		else
			p = p->right;
	}
	if (strcmp(word, q->word)<0)
		q->left = newNode(word, meaning);
	else
		q->right = newNode(word, meaning);


}

void addworden(node *tree, char *word, char *meaning)
{
	node *p, *q;
	p = q = tree;

	while (p != NULL)
	{
		q = p;
		if (strcmp(meaning, p->meaning)<0)
			p = p->left;
		else
			p = p->right;
	}
	if (strcmp(meaning, q->meaning)<0)
		q->left = newNode(word, meaning);
	else
		q->right = newNode(word, meaning);

}

struct node* TreeFromFile(FILE *f)
{
	node *ptree = NULL;
	char word[20], meaning[20], str[120], *i;
	int count = 0;

	if (f == NULL)
		ptree = NULL;
	else
	{
		while (!feof(f))
		{

			i = fgets(str, 120, f);

			if (i == NULL)
				break;

			separateword(str, word, meaning);
			if (count == 0)
			{
				ptree = newNode(word, meaning);
				count = 1;
			}
			else
				addword(ptree, word, meaning);
		}
		rewind(f);
	}
	return ptree;
}

struct node* TreeFromFileEn(FILE *f)
{
	node *ptree = NULL;
	char word[20], meaning[20], str[120], *i;
	int count = 0;

	if (f == NULL)
		ptree = NULL;
	else
	{
		while (!feof(f))
		{

			i = fgets(str, 120, f);

			if (i == NULL)
				break;

			separateword(str, word, meaning);
			if (count == 0)
			{
				ptree = newNode(word, meaning);
				count = 1;
			}
			else
				addworden(ptree, word, meaning);
		}
		rewind(f);
	}
	return ptree;
}

struct node *splayro(struct node *root, char *key)
{
	node N, *l, *r, *y;
	if (root == NULL) return root;
	N.left = N.right = NULL;
	l = r = &N;

	for (;;)
	{
		if (strcmp(key, root->word)<0)
		{
			if (root->left != NULL && strcmp(key, root->left->word)<0)
			{
				y = root->left;
				root->left = y->right;
				y->right = root;
				root = y;
			}
			if (root->left == NULL)
				break;
			r->left = root;
			r = root;
			root = root->left;
		}
		else if (strcmp(key, root->word)>0)
		{
			if (root->right != NULL && strcmp(key, root->right->word)>0)
			{
				y = root->right;
				root->right = y->left;
				y->left = root;
				root = y;
			}
			if (root->right == NULL)
				break;
			l->right = root;
			l = root;
			root = root->right;
		}
		else break;
	}

	l->right = root->left;
	root->left = root->right;
	root->left = N.right;
	root->right = N.left;
	return root;
}

struct node *splayen(struct node *root, char *key)
{
	node N, *l, *r, *y;
	if (root == NULL) return root;
	N.left = N.right = NULL;
	l = r = &N;

	for (;;)
	{
		if (strcmp(key, root->meaning)<0)
		{
			if (root->left != NULL && strcmp(key, root->left->meaning)<0)
			{
				y = root->left;
				root->left = y->right;
				y->right = root;
				root = y;
			}
			if (root->left == NULL) break;
			r->left = root;
			r = root;
			root = root->left;
		}
		else if (strcmp(key, root->meaning)>0)
		{
			if (root->right != NULL && strcmp(key, root->right->meaning)>0)
			{
				y = root->right;
				root->right = y->left;
				y->left = root;
				root = y;
			}
			if (root->right == NULL) break;
			l->right = root;
			l = root;
			root = root->right;
		}
		else break;
	}

	l->right = root->left;
	root->left = root->right;
	root->left = N.right;
	root->right = N.left;
	return root;
}

struct node *searchro(struct node *root, char *word)
{
	return splayro(root, word);
}

struct node*searchen(struct node *root, char *meaning)
{
	return splayen(root, meaning);
}


BOOL CALLBACK DlgDespre(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgDictionarRo(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgDictionarEn(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

HWND roButton, enButton;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		roButton = CreateWindow("BUTTON", "Romana->Engleza",
			WS_CHILD | WS_VISIBLE,
			395,
			70,
			200,
			100,
			hwnd,
			(HMENU)BUTTONRO,
			hInstance,
			NULL);
		enButton = CreateWindow("BUTTON", "Engleza->Romana",
			WS_CHILD | WS_VISIBLE,
			395,
			205,
			200,
			100,
			hwnd,
			(HMENU)BUTTONEN,
			hInstance,
			NULL);
		break;

		case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BUTTONRO:
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);
			return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DICTIONAR_RO), NULL, DlgDictionarRo);
		}
		break;

		case BUTTONEN:
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);
			return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DICTIONAR_EN), NULL, DlgDictionarEn);
		}
		break;

		case IDD_DICTIONAR_RO:
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);
			return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DICTIONAR_RO), NULL, DlgDictionarRo);
		}
		break;

		case IDD_DICTIONAR_EN:
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);
			return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DICTIONAR_EN), NULL, DlgDictionarEn);
		}
		break;

		case IDD_DESPRE:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DESPRE), hwnd, DlgDespre);
			break;

		case ID_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		break;

		case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

		case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDD_MAIN);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,g_szClassName,"Dictionar RO-EN/EN-RO",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT, 700, 450,NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
BOOL CALLBACK DlgDespre(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_PAINT:
		{
		HDC hDC;
		PAINTSTRUCT ps;
		HPEN hOldPen, hNewPen;
		hDC = BeginPaint(hwnd, &ps);
		hNewPen = CreatePen(PS_SOLID, 1, RGB(0, 180, 180));
		hOldPen = (HPEN)SelectObject(hDC, hNewPen);
		TextOut(hDC, 0, 120, "Profesor coordonator: Mihai Lica Pura                           ", 130);
		MoveToEx(hDC, 0, 115, NULL);
		LineTo(hDC, 700, 115);
		}
		break;

		case WM_INITDIALOG:
		return TRUE;

		case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		}
		break;

		default:
		return FALSE;
	}
	return TRUE;
}
BOOL CALLBACK DlgDictionarRo(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_PAINT:
		{
		HDC hDC;
		PAINTSTRUCT ps;
		HPEN hOldPen, hNewPen;
		hDC = BeginPaint(hwnd, &ps);
		hNewPen = CreatePen(PS_SOLID, 1, RGB(0, 180, 180));
		hOldPen = (HPEN)SelectObject(hDC, hNewPen);
		TextOut(hDC, 0, 216, "    Stud. Avram Rares-Alexandru                                                                                   Grupa E211B", 130);
		MoveToEx(hDC, 0, 212, NULL);
		LineTo(hDC, 700, 212);
		}
		break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_ADD_STRING:
				{
					node *tree_root = TreeFromFile(f);
					int i, n = 0;
					char *p, v[100][100], s[100], string[100] = "";
					int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT1));
					if (len > 0)
					{
						char* buf;
						buf = (char*)malloc(len + 1);

						GetDlgItemText(hwnd, IDC_TEXT1, buf, len + 1);

						p = strtok(buf, " ");

						while (p != NULL)
						{
							strcpy(v[n], p);
							n++;
							p = strtok(NULL, " ");
						}

						for (i = 0; i<n; i++)
						{
							node* temp = splayro(tree_root, v[i]);
							tree_root = TreeFromFile(f);

							if (strcmp(v[i], temp->word) == 0)
							{
								strcat(string, temp->meaning);
								strcat(string, " ");
							}
						}

						SetDlgItemText(hwnd, IDC_TEXT2, string);
					}
			    }
				break;

				case IDC_CLEAR1:
				{
					free(tree_root);
					SetDlgItemText(hwnd, IDC_TEXT1, "");
					SetDlgItemText(hwnd, IDC_TEXT2, "");
				}
				break;
			}
		break;

		case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

		default:
		return FALSE;
	}
	return TRUE;
}
BOOL CALLBACK DlgDictionarEn(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_PAINT:
		{
		HDC hDC;
		PAINTSTRUCT ps;
		HPEN hOldPen, hNewPen;
		hDC = BeginPaint(hwnd, &ps);
		hNewPen = CreatePen(PS_SOLID, 1, RGB(0, 180, 180));
		hOldPen = (HPEN)SelectObject(hDC, hNewPen);
		TextOut(hDC, 0, 216, "    Stud. Avram Rares-Alexandru                                                                                   Grupa E211B", 130);
		MoveToEx(hDC, 0, 212, NULL);
		LineTo(hDC, 700, 212); 
		}
		break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_ADD_STRING1:
				{
					node *tree_root = TreeFromFileEn(f);
					int i, n = 0;
					char *p, v[100][100], s[100], string[100] = "";
					int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT4));
					if (len > 0)
					{
						char* buf;
						buf = (char*)malloc(len + 1);

						GetDlgItemText(hwnd, IDC_TEXT4, buf, len + 1);

						p = strtok(buf, " ");

						while (p != NULL)
						{
							strcpy(v[n], p);
							n++;
							p = strtok(NULL, " ");
						}

						for (i = 0; i < n; i++)
						{
							node* temp = splayen(tree_root, v[i]);
							tree_root = TreeFromFileEn(f);

							if (strcmp(v[i], temp->meaning) == 0)
							{
								strcat(string, temp->word);
								strcat(string, " ");
							}
						}

						SetDlgItemText(hwnd, IDC_TEXT3, string);
					}
				}
			break;

			case IDC_CLEAR1:
			{
				free(tree_root);
				SetDlgItemText(hwnd, IDC_TEXT3, "");
				SetDlgItemText(hwnd, IDC_TEXT4, "");
			}
			break;
			}
		break;

		case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
		
		default:
		return FALSE;
	}
	return TRUE;
}