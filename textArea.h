#ifndef _TEXTAREA_H_
#define _TEXTAREA_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>

    #define FALSE 0
    #define TRUE 1

    /* structs */

    typedef struct TextArea {

        // the color of the background
        SDL_Color bgColor;

        // the color of the text
        SDL_Color textColor;

        // the rect of the textArea
        SDL_Rect rect;

        // the pointer to the renderer
        SDL_Renderer* renderer;

        // the pointer to the font
        TTF_Font* font;

        // the pos of the cursor
        int cursorX, cursorY;

        // init colors 
        SDL_Color bgInitColor, textInitColor;

        // if the cursor is inside the bgRect
        int inside;

        // the text in the box
        char* text;

        // the lenght of text
        int textLen;

        // the texture of the text
        SDL_Texture* Text;

        //rects of the text texture
        SDL_Rect srcRect, destRect;

        // if the textArea is deleted
        Uint8 deleted;

        // if the delet button is pushed
        Uint8 del;

        // the adding x if the text is too larg
        int  addingX;

        // if the textArea is loked
        Uint8 locked;

        // the max size of char if -1 then infinit char
        int maxSize;

    } TextArea ;

    /* prototypes */
        // init a textArea
        int isNULLTextArea(TextArea* TA);
        TextArea* initTextArea(SDL_Renderer* renderer, TTF_Font* font);
        TextArea* setBGColor(TextArea* TA, SDL_Color color);
        TextArea* setTextAreaColor(TextArea* TA, SDL_Color color);
        TextArea* delTextArea(TextArea* TA);
        TextArea* setTextAreaPos(TextArea* TA, int x, int y);
        TextArea* setTextAreaSize(TextArea* TA, int w, int h);
        TextArea* setTextAreaRect(TextArea* TA, SDL_Rect rect);
        TextArea* setTextAreaCursor(TextArea* TA, int x, int y);
        TextArea* lockTextArea(TextArea* TA);
        TextArea* setMaxTextArea(TextArea* TA, int maxSize);

        int drawTextArea(TextArea* TA);
        int eventTextArea(TextArea* TA, SDL_Event* event);
        TextArea* updateTextArea(TextArea* TA);

/* set prototypes */

int isInside(SDL_Rect rect, SDL_Point point){
    if (point.x > rect.x && point.x < rect.x + rect.w){
        if (point.y > rect.y && point.y < rect.y + rect.h){
            return TRUE;
        }
    }
    return FALSE;
}

int isNULLTextArea(TextArea* TA){
    if (TA == NULL){
        return TRUE;
    } else {
        return FALSE;
    }
}

TextArea* initTextArea(SDL_Renderer* renderer, TTF_Font* font){
    TextArea* TA = NULL;
    TA = (TextArea*) malloc(sizeof(TextArea));

    if (TA == NULL){
        fprintf(stderr, "ERROR : during malloc of 'TextArea' type\n");
        return NULL;
    }

    if (renderer == NULL){
        printf("cannnot set a NULL renderer\n");
        TA = delTextArea(TA);
        return NULL;
    }
    TA->renderer = renderer;

    if (font == NULL){
        printf("cannot set a NULL font\n");
        TA = delTextArea(TA);
        return NULL;
    }
    TA->font = font;

    TA->bgColor = (SDL_Color){0, 0, 0, 0};
    TA->textColor = (SDL_Color){255, 255, 255, 0};
    TA->rect = (SDL_Rect){0, 0, 0, 0};
    TA->text = NULL;

    TA->text = (char*) malloc(sizeof(char) * 255);
    if (TA->text == NULL){
        fprintf(stderr, "ERROR : malloc of TA->text\n");
    }

    strcpy(TA->text, "");

    TA->Text = NULL;
    TA->srcRect = (SDL_Rect){0, 0, 0, 0};
    TA->destRect = (SDL_Rect){0, 0, 0, 0};
    TA->textLen = 0;
    TA->inside = FALSE;
    TA->deleted = FALSE;
    TA->addingX = 0;
    TA->locked = FALSE;
    TA->maxSize = -1;

    // else
    return TA;
}

TextArea* setBGColor(TextArea* TA, SDL_Color color){
    if(!isNULLTextArea(TA)){
        TA->bgColor = color;
        TA->bgInitColor = color;

    } else {
        printf("cannot set the color of a NULL textArea\n");
        return NULL;
    }
    return TA;
}

TextArea* setTextColor(TextArea* TA, SDL_Color color){
    if(!isNULLTextArea(TA)){
        TA->textColor = color;
        TA->textInitColor = color;
    } else {
        printf("cannot set the color of a NULL textArea\n");
        return NULL;
    }

    return TA; 
}

TextArea* delTextArea(TextArea* TA){
    if (!isNULLTextArea(TA)){
        free(TA);

        TA->font = NULL;
        TA->renderer = NULL;
        TA->deleted = TRUE;

        free(TA->text);
        TA->text = NULL;

        SDL_DestroyTexture(TA->Text);
        TA->Text = NULL;
    }
    return TA;
}


TextArea* setTextAreaPos(TextArea* TA, int x, int y){
    if (!isNULLTextArea(TA)){
        TA->rect.x = x;
        TA->rect.y = y;

        TA->destRect.x = (TA->rect.w - TA->destRect.w) / 2 + TA->rect.x + TA->addingX;
        TA->destRect.y = (TA->rect.h - TA->destRect.h) / 2 + TA->rect.y;

    } else {
        printf("cannot set the pos of a NULL textArea\n");
        return NULL;
    }
    return TA;
}

TextArea* setTextAreaSize(TextArea* TA, int w, int h){
    if (!isNULLTextArea(TA)){
        TA->rect.w = w;
        TA->rect.h = h;

        TA->srcRect.w = w;
        TA->srcRect.h = h;

    } else {
        printf("cannot set the size of a NULL textArea\n");
        return NULL;
    }
    return TA;
}

TextArea* setTextAreaRect(TextArea* TA, SDL_Rect rect){
    if (!isNULLTextArea(TA)){
        TA->rect = rect;
        TA->destRect.x = rect.x;
        TA->destRect.y = rect.y;

        TA->destRect.x = (TA->rect.w - TA->destRect.w) / 2 + TA->rect.x + TA->addingX;
        TA->destRect.y = (TA->rect.h - TA->destRect.h) / 2 + TA->rect.y;

    } else {
        printf("cannot set the rect of a NULL textArea\n");
        return NULL;
    }
    return TA;
}

TextArea* setTextAreaCursor(TextArea* TA, int x, int y){
    if(!isNULLTextArea(TA)){
        TA->cursorX = x;
        TA->cursorY = y;

        if (isInside(TA->rect, (SDL_Point){TA->cursorX, TA->cursorY})){
            TA->inside = TRUE;
        } else {
            TA->inside = FALSE;
        }
    } else {
        printf("cannot set the cursor pos of a NULL textArea\n");
        return NULL;
    }
    return TA;
}

TextArea* lockTextArea(TextArea* TA){
    if (TA->locked == TRUE){
        TA->locked = FALSE;
    } else {
        TA->locked = TRUE;
    }
    return TA;
}

TextArea* setMaxTextArea(TextArea* TA, int maxSize){
    TA->maxSize = maxSize;
    return TA;
}

int drawTextArea(TextArea* TA){
    if (!isNULLTextArea(TA)){
        if (SDL_SetRenderDrawColor(TA->renderer, TA->bgColor.r, TA->bgColor.g, TA->bgColor.b, TA->bgColor.a) == 0){
            if (SDL_RenderFillRect(TA->renderer, &TA->rect) == 0){
                
                if (TA->textLen > 0){
                    SDL_Rect rect = {0, 0, TA->rect.w, TA->rect.h};
                    SDL_Rect dstRect;
                    
                    if (TA->destRect.x > TA->rect.x){
                        dstRect.x = TA->destRect.x;
                    } else {
                        dstRect.x = TA->rect.x;
                    }
                    if (TA->destRect.w < TA->rect.w){
                        dstRect.w = TA->destRect.w;
                    } else {
                        dstRect.w = TA->rect.w;
                    }
                    dstRect.y = TA->destRect.y;
                    dstRect.h = TA->destRect.h;

                    SDL_RenderCopy(TA->renderer, TA->Text, &rect, &dstRect);
                }
            } else {
                fprintf(stderr, "SDL_RenderDrawRect ERROR : %s\n", SDL_GetError());
                return FALSE;
            }
        } else {
            fprintf(stderr, "SDL_SetRenderDrawColor ERROR : %s\n", SDL_GetError());
            return FALSE;
        }
    } else {
        printf("cannot draw a NULL textArea\n");
        return FALSE;
    }
    // if success
    return TRUE;
}

int setTextAreaText(TextArea* TA){
    if (TA->textLen > 0){

        SDL_Surface* surface = TTF_RenderText_Solid(TA->font, TA->text, TA->textColor);

        if (surface == NULL){
            printf("TTF_RenderText_Solid error : %s\n", TTF_GetError());
            return FALSE;
        }

        SDL_DestroyTexture(TA->Text);
        TA->Text = SDL_CreateTextureFromSurface(TA->renderer, surface);

        if (TA->Text != NULL){

            if (SDL_QueryTexture(TA->Text, NULL, NULL, &TA->destRect.w, &TA->destRect.h) == 0){
                
                TA->destRect.x = (TA->rect.w - TA->destRect.w) / 2 + TA->rect.x + TA->addingX;
                TA->destRect.y = (TA->rect.h - TA->destRect.h) / 2 + TA->rect.y;
                
            } else {
                printf("SDL_QueryTexture Error : %s", SDL_GetError());
                return FALSE;
            }
        } else {
            printf("SDL_CreateTextureFromSurface Error : %S", SDL_GetError());
            return FALSE;
        }
        SDL_FreeSurface(surface);
    } 

    return TRUE;
}

int eventTextArea(TextArea* TA, SDL_Event* event){

    if (TA->inside == TRUE && TA->locked == FALSE){
        if(event->type == SDL_TEXTINPUT){

            int size = strlen(event->text.text) + TA->textLen;

            if (size < TA->maxSize || TA->maxSize == -1){
                TA->textLen = size;
                strcat(TA->text, event->text.text);
                return setTextAreaText(TA);
            }
            
        } else if (event->key.keysym.scancode == 42){
            if (TA->textLen > 0 && TA->del != TRUE){
                TA->text[TA->textLen-1] = '\0';
                TA->textLen--;

                return setTextAreaText(TA);
            }
            TA->del = TRUE;
        } else {
            TA->del = FALSE;
        }
    }

    return TRUE;
}

TextArea* updateTextArea(TextArea* TA){
    if (!isNULLTextArea(TA)){
        if (TA->inside == FALSE || TA->locked == TRUE){
            TA->bgColor.r = TA->bgInitColor.r / 2;
            TA->bgColor.g = TA->bgInitColor.g / 2;
            TA->bgColor.b = TA->bgInitColor.b / 2;
            TA->bgColor.a = TA->bgInitColor.a / 2;
        } else {
            TA->bgColor = TA->bgInitColor;
        }
    } else {
        return NULL;
    }
    return TA;
}

#endif