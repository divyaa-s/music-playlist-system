#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a song in the playlist
struct Song {
    char title[100];
    struct Song* next;
};

// Structure to represent the playlist as a stack
struct Playlist {
    struct Song* top;
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

// Function to initialize the playlist
void initializePlaylist(struct Playlist* playlist) {
    playlist->top = NULL;
}

// Function to push (add) a song to the playlist
void pushSong(struct Playlist* playlist, const char* title) {
    struct Song* song = createSong(title);
    if (song == NULL) {
        printf("Error: Failed to create a new song.\n");
        return;
    }

    song->next = playlist->top; // Add the new song to the top of the stack
    playlist->top = song;

    printf("Added \"%s\" to the playlist.\n", title);
}

// Function to pop (remove) the current song from the playlist
void popSong(struct Playlist* playlist) {
    if (playlist->top != NULL) {
        struct Song* current = playlist->top;
        playlist->top = current->next; // Remove the top song
        printf("Deleted \"%s\" from the playlist.\n", current->title);
        free(current);
    }
}

// Function to shuffle the playlist
void shufflePlaylist(struct Playlist* playlist) {
    // Create an array to store songs in the playlist
    struct Song* songs[100]; // Assuming a maximum of 100 songs
    int songCount = 0;

    // Traverse the playlist and store songs in the array
    struct Song* current = playlist->top;
    while (current != NULL) {
        songs[songCount] = current;
        current = current->next;
        songCount++;
    }

    if (songCount <= 1) {
        printf("Shuffling is not needed with only one song or an empty playlist.\n");
        return;
    }

    // Seed the random number generator with the current time
    srand(time(NULL));

    // Shuffle the songs using the Fisher-Yates algorithm
    for (int i = songCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Song* temp = songs[i];
        songs[i] = songs[j];
        songs[j] = temp;
    }

    // Rebuild the playlist with the shuffled order
    playlist->top = songs[0];
    for (int i = 1; i < songCount; i++) {
        songs[i - 1]->next = songs[i];
    }
    songs[songCount - 1]->next = NULL;

    printf("Playlist shuffled.\n");
}

// Function to display the current playlist
void displayPlaylist(const struct Playlist* playlist) {
    printf("Current Playlist:\n");
    struct Song* current = playlist->top;
    while (current != NULL) {
        printf("%s\n", current->title);
        current = current->next;
    }
    printf("End of Playlist\n");
}

// Function to free the memory used by the playlist
void freePlaylist(struct Playlist* playlist) {
    struct Song* current = playlist->top;
    while (current != NULL) {
        struct Song* temp = current;
        current = current->next;
        free(temp);
    }
    playlist->top = NULL;
}

int main() {
    struct Playlist playlist;
    initializePlaylist(&playlist);

    int choice;
    char title[100];

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
                scanf("%s", title);
                pushSong(&playlist, title);
                break;
            case 2:
                popSong(&playlist);
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
