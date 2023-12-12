#ifndef INTERVAL_H
#define INTERVAL_H


class Interval {
public:
	double min, max;

	Interval() : min(INFINITY), max(-INFINITY) {};
	Interval(double _min, double _max) : min(_min), max(_max) {}
	Interval(Interval _i0, Interval _i1) : min(std::min(_i0.min, _i1.min)), max(std::max(_i0.max, _i1.max)) {}

	const void thicken() { min = min - 0.0001f; max = max + 0.0001f; }

	inline Interval merge(const Interval& _other) const { return Interval(std::min(min, _other.min), std::min(max, _other.max)); }

	inline bool overlap(const Interval& _other) const { return (min <= _other.max && _other.min <= max); }
	inline bool consists(const double& point) const { return (min <= point && max >= point); }
	inline double getLength() const { return max - min; }
	
	
};
#endif // !INTERVAL_H
