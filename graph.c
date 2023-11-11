#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a song in the playlist
struct Song {
    char title[100];
};

// Structure to represent an edge between songs (the order of songs in the playlist)
struct Edge {
    struct Song* song;
    struct Edge* next;
};

// Structure to represent a playlist
struct Playlist {
    struct Edge* head;
};

// Function to create a new song
struct Song* createSong(const char* title) {
    struct Song* song = (struct Song*)malloc(sizeof(struct Song));
    if (song != NULL) {
        strncpy(song->title, title, sizeof(song->title));
    }
    return song;
}

// Function to create a new edge between songs
struct Edge* createEdge(struct Song* song) {
    struct Edge* edge = (struct Edge*)malloc(sizeof(struct Edge));
    if (edge != NULL) {
        edge->song = song;
        edge->next = NULL;
    }
    return edge;
}

// Function to add a song to the playlist
void addSong(struct Playlist* playlist, const char* title) {
    struct Song* song = createSong(title);
    if (song == NULL) {
        printf("Error: Failed to create a new song.\n");
        return;
    }

    struct Edge* edge = createEdge(song);
    if (edge == NULL) {
        free(song);
        printf("Error: Failed to create a new edge.\n");
        return;
    }

    if (playlist->head == NULL) {
        playlist->head = edge;
    } else {
        struct Edge* current = playlist->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = edge;
    }
    printf("Added \"%s\" to the playlist.\n", title);
}

// Function to delete a song from the playlist
void deleteSong(struct Playlist* playlist, const char* title) {
    struct Edge* current = playlist->head;
    struct Edge* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->song->title, title) == 0) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                playlist->head = current->next;
            }
            free(current->song);
            free(current);
            printf("Deleted \"%s\" from the playlist.\n", title);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Song \"%s\" not found in the playlist.\n", title);
}

// Function to shuffle the playlist (randomize the order of songs)
void shufflePlaylist(struct Playlist* playlist) {
    // Create an array of pointers to edges (songs)
    int songCount = 0;
    struct Edge* current = playlist->head;
    while (current != NULL) {
        songCount++;
        current = current->next;
    }

    if (songCount < 2) {
        printf("Not enough songs to shuffle.\n");
        return;
    }

    struct Edge** songArray = (struct Edge**)malloc(songCount * sizeof(struct Edge*));
    if (songArray == NULL) {
        printf("Memory allocation error for shuffle.\n");
        return;
    }

    current = playlist->head;
    int index = 0;
    while (current != NULL) {
        songArray[index] = current;
        current = current->next;
        index++;
    }

    // Perform the Fisher-Yates shuffle
    srand((unsigned int)time(NULL)); 
// Seed the random number generator

    for (int i = songCount - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Generate a random index between 0 and i
        if (i != j) {
            // Swap songs at index i and j in the songArray
            struct Edge* temp = songArray[i];
            songArray[i] = songArray[j];
            songArray[j] = temp;
        }
    }

    // Update the playlist with the shuffled order
    playlist->head = songArray[0];
    current = playlist->head;
    for (int i = 1; i < songCount; i++) {
        current->next = songArray[i];
        current = current->next;
    }
    current->next = NULL;

    free(songArray);
    printf("Playlist shuffled.\n");
}



// Function to display the current playlist
void displayPlaylist(const struct Playlist* playlist) {
    printf("Current Playlist:\n");
    struct Edge* current = playlist->head;
    while (current != NULL) {
        printf("%s\n", current->song->title);
        current = current->next;
    }
}

// Function to free the memory used by the playlist
void freePlaylist(struct Playlist* playlist) {
    struct Edge* current = playlist->head;
    while (current != NULL) {
        struct Edge* tempEdge = current;
        current = current->next;
        free(tempEdge->song);
        free(tempEdge);
    }
    playlist->head = NULL;
}

int main() {
    struct Playlist playlist = {NULL};

    int choice;
    char title[100];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add a song\n");
        printf("2. Delete a song\n");
        printf("3. Shuffle the playlist\n");
        printf("4. Display the playlist\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the title of the song: ");
                scanf("%s", title);
                addSong(&playlist, title);
                break;
            case 2:
                printf("Enter the title of the song to delete: ");
                scanf("%s", title);
                deleteSong(&playlist, title);
                break;
            case 3:
                shufflePlaylist(&playlist);
                break;
            case 4:
                displayPlaylist(&playlist);
                break;
            case 5:
                freePlaylist(&playlist);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
