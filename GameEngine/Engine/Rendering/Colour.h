class Colour
{
private:
	int red;
	int green;
	int blue;
	int alpha;

public:
	Colour(int r, int g, int b);
	Colour(int r, int g, int b, int a);

	int getRed() const;
	int getGreen() const;
	int getBlue() const;
	int getAlpha() const;

	void setRed(int r);
	void setGreen(int g);
	void setBlue(int b);
	void setAlpha(int a);
};