#include <iostream>
#include <sstream>
#include <iomanip>

constexpr double kMinTestSpeed = 2.5;
constexpr double kMaxTestSpeed = 50.0;
constexpr double kSpeedStep = 2.5;
constexpr int kNoTestSpeed = static_cast<int>((kMaxTestSpeed - kMinTestSpeed) / kSpeedStep + 1.0);

using namespace std;

void textout_invalid_data()
{
	cout << setw(6) << "x";
}

void textout_leadoff_range(double minimum_leadoff, double maximum_leadoff)
{
	cout << setw(3) << static_cast<int>(minimum_leadoff)
		<< "-"
		<< setw(2) << static_cast<int>(maximum_leadoff + 0.99);
}

void analyze()
{
	string data;
	getline(cin, data);
	// A data to analyze, formatted like;
	// "distance/frame distance/frame distance/frame"; of any number of samples.

	istringstream stream{ data };

	// writes column header; bullet speeds.
	{
		cout << setw(7) << "[m/f]";
		for (int i = 0; i < kNoTestSpeed; ++i)
		{
			cout << setw(6) << fixed << setprecision(1) << (i * kSpeedStep + kMinTestSpeed);
		}
		cout << endl;
	}

	for (;;)
	{
		int frame, length;
		char separator;

		stream >> length >> separator >> frame;
		if (stream.fail())
			break;

		// writes row header; the data.
		cout << setw(3) << length << "/" << setw(2) << frame << " ";

		// writes leadoff estimated if it's valid data
		for (int i = 0; i < kNoTestSpeed; ++i)
		{
			double speed = kMinTestSpeed + i * 2.5;

			// the bullet should have hit the target earlier.
			if (length <= speed * (frame - 1))
			{
				textout_invalid_data();
			}

			// the bullet have hit the target thanks to some leadoff distance.
			else if (length > speed * frame)
			{
				if (auto leadoff_min = length - speed * frame; leadoff_min < 50)
				{
					textout_leadoff_range(leadoff_min, leadoff_min + speed);
				}
				else
				{
					textout_invalid_data();
				}
			}

			// the bullet haven't hit the target before that frame.
			else
			{
				textout_leadoff_range(0.0, (frame > 1) ? length - speed * (frame - 1) : 99.0);
			}
		}

		cout << endl;
	}
}



int main()
{
	for (;;)
	{
		analyze();
	}
    return 0;
}

