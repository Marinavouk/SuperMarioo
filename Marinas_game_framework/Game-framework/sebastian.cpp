
// An example of how you could create a tilemap...


struct Tile {
	int id;
	bool solid;
}

class Map {

	private const int tile_size = 32;

	public width = 12;	//in tiles
	public height = 12; // in pixels
	public Tile *data;

	Texture tileset;	//holds the image that holds all the tiles

	Map(int width, int height) {
		this.width = width;
		this.height = height;
		data = (Tile*)malloc(width * height * sizeof(Tile));
	}

	~Map() {
		free(data);
		data = nullptr;
	}

	public void SetTile(int x, int y, int tile_id, bool solid) {
		data[y * map_width + x].id = tile_id;
		data[y * map_width + x].id = solid;
	}

	public bool IsSolid(int x, int y) {
		return data[y * map_width + x].solid
	}

	public void Render() {
		for (int y = 0; y < map_width; y++) {
			for (int x = 0; x < map_height; x++) {
				Vector2 tile_pos = { x * tile_size, y * tile_size };
				
				switch (data[y * map_width + x]) {
				case 1:
				{
					// draw a block
				}
				case 2:
				{
					// draw a pipe
				}
				
			}
		}
	}
};
	}


 // Concept of animations and sprites:

class Animation {

	Texture animation_image_file;
	int number_of_frames;
	int frames_per_second;
	int max_frames;
};

class Sprite {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	std::vector<Animation> animations;
	int currrentAnimation;
	int currentFrame;
	float lastUpdated;

	void Update(float delta_time) {
		lastUpdated += delta_time;
		if (lastUpdated > 1.0f / animations[currentAnimation].frames_per_second) {
			lastUpdated = 0.0f;
			currentFrame += 1;
			if (currentFrame > animations[currentAnimation].max_frames) {
				currentFrame = 0;
			}
		}

		velocity.x += acceleration.x * delta_time;
		velocity.y += acceleration.y * delta_time;
		position.x += velocity.x * delta_time;
		position.y += velocity.y * delta_time;
	}

	void Draw() {

	}

}