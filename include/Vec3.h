#ifndef VEC3_H
#define	VEC3_H

/**
 * \class Vec3
 * \brief Represents a 3-dimensional vector (X, Y, Z)
 */
class Vec3 {
    public:
        static const int X = 0;
        static const int Y = 1;
        static const int Z = 2;

        Vec3();
        Vec3(float x, float y, float z);

        Vec3 operator -(const Vec3& vector) const;
        Vec3 operator +(const Vec3& vector) const;
        Vec3 operator *(float scalar) const;
        Vec3& operator -=(const Vec3& vector);
        Vec3& operator +=(const Vec3& vector);
        Vec3& operator *=(float scalar);
        bool operator ==(const Vec3& vector) const;
        bool operator !=(const Vec3& vector) const;
        Vec3 operator -() const;
        
        float dot(const Vec3& vector) const;
        Vec3 cross(const Vec3& vector) const;
        Vec3& normalize();
        float length() const;
        float squareLength() const;

        float get(int index) const;
        void set(int index, float value);
        
        const float* data() const  {
            return (float*)&this->vector;
        }

    private:
        float vector[3];
};

#endif	/* VEC3_H */
