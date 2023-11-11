#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a song in the playlist
struct Song {
    char title[100];
    struct Song* next;
};

// Function to create a new song
struct Song* createSong(const char* title) {
    struct Song* song = (struct Song*)malloc(sizeof(struct Song));
    if (song != NULL) {
        strncpy(song->title, title, sizeof(song->title));
        song->next = NULL;
    }
    return song;
}

// Structure to represent the circular linked list playlist
struct Playlist {
    struct Song* current;
};

// Function to initialize the playlist
void initializePlaylist(struct Playlist* playlist) {
    playlist->current = NULL;
}

// Function to add a song to the playlist
void addSong(struct Playlist* playlist, const char* title) {
    struct Song* song = createSong(title);
    if (song == NULL) {
        printf("Error: Failed to create a new song.\n");
        return;
    }

    if (playlist->current == NULL) {
        // If the playlist is empty, set the current song to the new song
        playlist->current = song;
        song->next = song; // Circular reference
    } else {
        // Insert the new song after the current song
        song->next = playlist->current->next;
        playlist->current->next = song;
    }
    printf("Added \"%s\" to the playlist.\n", title);
}

// Function to delete the current song from the playlist
void deleteCurrentSong(struct Playlist* playlist) {
    if (playlist->current != NULL) {
        struct Song* current = playlist->current;

        // Find the previous song
        struct Song* prev = playlist->current;
        while (prev->next != playlist->current) {
            prev = prev->next;
        }

        prev->next = current->next; // Remove the current song
        if (current == current->next) {
            // If it was the last song, set current to NULL
            playlist->current = NULL;
        } else {
            playlist->current = current->next; // Move to the next song
        }

        printf("Deleted \"%s\" from the playlist.\n", current->title);
        free(current);
    }
}

// Function to shuffle the playlist
void shufflePlaylist(struct Playlist* playlist) {
    if (playlist->current != NULL) {
        // Seed the random number generator with the current time
        srand(time(NULL));

        int steps = rand() % 10 + 1; // Random number of steps to shuffle
        for (int i = 0; i < steps; i++) {
            playlist->current = playlist->current->next; // Move to the next song
        }

        printf("Playlist shuffled.\n");
    }
}

// Function to display the current playlist
void displayPlaylist(const struct Playlist* playlist) {
    if (playlist->current != NULL) {
        struct Song* current = playlist->current;
        do {
            printf("%s\n", current->title);
            current = current->next;
        } while (current != playlist->current);
    }
    printf("End of Playlist\n");
}

// Function to free the memory used by the playlist
void freePlaylist(struct Playlist* playlist) {
    if (playlist->current != NULL) {
        struct Song* current = playlist->current;
        struct Song* next;

        do {
            next = current->next;
            free(current);
            current = next;
        } while (current != playlist->current);

        playlist->current = NULL;
    }
}

int main() {
    struct Playlist playlist;
    initializePlaylist(&playlist);

    int choice;
    char title[100]; // Changed to 100, not 500

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add a song\n");
        printf("2. Delete the current song\n");
        printf("3. Shuffle the playlist\n");
        printf("4. Display the playlist\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the title of the song: ");
                scanf("%s", title); // Removed the '\n'
                addSong(&playlist, title);
                break;
            case 2:
                deleteCurrentSong(&playlist);
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
