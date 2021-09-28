#version 330

out vec4 color;

uniform vec2 resolution;


// ray marching
const int max_iterations = 512;
const float stop_threshold = 0.001;
const float grad_step = 0.02;
const float clip_far = 100.0;

// math
const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;

float __rand(float a, float b)
{
    return a + b - sqrt(pow(a, 2) + pow(b, 2));
}
float __ror(float a, float b)
{
    return a + b + sqrt(pow(a, 2) + pow(b, 2));
}
float __resultFunc(float x0, float y0, float z0)
{
    const float e = 2.71828;
    const float pi = 3.14159;
    float x =(x0);
    float y =(y0);
    float z =(z0);
    float s =((((16.000000)-(pow((x), (2.000000))))-(pow((y), (2.000000))))-(pow((z), (2.000000))));
    float a =((3.000000)-(abs((x))));
    float b =((3.000000)-(abs((y))));
    float c =((3.000000)-(abs((z))));
    float k =(__rand((__rand((a), (b))), (c)));
    float sh =(__rand((s), (k)));
    float z13 =(pow(((z)-(1.300000)), (2.000000)));
    float z13p =(pow(((z)+(1.300000)), (2.000000)));
    float y13 =(pow(((y)-(1.300000)), (2.000000)));
    float y13p =(pow(((y)+(1.300000)), (2.000000)));
    float z3p =(pow(((z)+(3.000000)), (2.000000)));
    float z3 =(pow(((z)-(3.000000)), (2.000000)));
    float x3 =(pow(((x)+(3.000000)), (2.000000)));
    float x13 =(pow(((x)-(1.300000)), (2.000000)));
    float y3 =(pow(((y)+(3.000000)), (2.000000)));
    float x13p =(pow(((x)+(1.300000)), (2.000000)));
    float t11 =((((0.300000)-(pow(((x)-(3.000000)), (2.000000))))-(pow((y), (2.000000))))-(pow((z), (2.000000))));
    float t21 =((((0.300000)-(x13))-(pow(((y)-(3.000000)), (2.000000))))-(z13));
    float t22 =((((0.300000)-(x13p))-(pow(((y)-(3.000000)), (2.000000))))-(z13p));
    float t31 =((((0.300000)-(x13))-(y13))-(z3));
    float t32 =((((0.300000)-(pow((x), (2.000000))))-(pow((y), (2.000000))))-(z3));
    float t33 =((((0.300000)-(x13p))-(y13p))-(z3));
    float t41 =((((0.300000)-(x13))-(y13))-(z3p));
    float t42 =((((0.300000)-(x13p))-(y13))-(z3p));
    float t43 =((((0.300000)-(x13))-(y13p))-(z3p));
    float t44 =((((0.300000)-(x13p))-(y13p))-(z3p));
    float t51 =((((0.300000)-(pow((x), (2.000000))))-(y3))-(pow((z), (2.000000))));
    float t52 =((((0.300000)-(x13))-(y3))-(z13));
    float t53 =((((0.300000)-(x13))-(y3))-(z13p));
    float t54 =((((0.300000)-(x13p))-(y3))-(z13));
    float t55 =((((0.300000)-(x13p))-(y3))-(z13p));
    float t61 =((((0.300000)-(x3))-(y13))-(z13));
    float t62 =((((0.300000)-(x3))-(pow((y), (2.000000))))-(z13));
    float t63 =((((0.300000)-(x3))-(y13p))-(z13));
    float t64 =((((0.300000)-(x3))-(y13))-(z13p));
    float t65 =((((0.300000)-(x3))-(pow((y), (2.000000))))-(z13p));
    float t66 =((((0.300000)-(x3))-(y13p))-(z13p));
    float w =(__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((__rand((sh),(-(t11)))), (-(t21)))), (-(t22)))), (-(t31)))), (-(t32)))), (-(t33)))), (-(t41)))), (-(t42)))), (-(t43)))), (-(t44)))), (-(t51)))),
                                                                                             (-(t52)))), (-(t53)))), (-(t54)))), (-(t55)))), (-(t61)))), (-(t62)))), (-(t63)))), (-(t64)))), (-(t65)))), (-(t66))));
    return (w);
}

// get distance in the world
float dist_field( vec3 p ) {
    return -__resultFunc(p.x, p.y, p.z);
}

// get gradient in the world
vec3 gradient( vec3 pos ) {
    const vec3 dx = vec3( grad_step, 0.0, 0.0 );
    const vec3 dy = vec3( 0.0, grad_step, 0.0 );
    const vec3 dz = vec3( 0.0, 0.0, grad_step );
    return normalize (
                vec3(
                    dist_field( pos + dx ) - dist_field( pos - dx ),
                    dist_field( pos + dy ) - dist_field( pos - dy ),
                    dist_field( pos + dz ) - dist_field( pos - dz )
                    )
                );
}

vec3 fresnel( vec3 F0, vec3 h, vec3 l ) {
    return F0 + ( 1.0 - F0 ) * pow( clamp( 1.0 - dot( h, l ), 0.0, 1.0 ), 5.0 );
}

// phong shading
vec3 shading( vec3 v, vec3 n, vec3 dir, vec3 eye ) {
    // ...add lights here...

    float shininess = 16.0;

    vec3 final = vec3( 0.0 );

    vec3 ref = reflect( dir, n );

    vec3 Ks = vec3( 0.5 );
    vec3 Kd = vec3( 1.0 );

    // light 0
    {
        vec3 light_pos   = vec3( 20.0, 20.0, 20.0 );
        vec3 light_color = vec3( 1.0, 0.7, 0.7 );

        vec3 vl = normalize( light_pos - v );

        vec3 diffuse  = Kd * vec3( max( 0.0, dot( vl, n ) ) );
        vec3 specular = vec3( max( 0.0, dot( vl, ref ) ) );

        vec3 F = fresnel( Ks, normalize( vl - dir ), vl );
        specular = pow( specular, vec3( shininess ) );

        final += light_color * mix( diffuse, specular, F );
    }

    // light 1
    {
        vec3 light_pos   = vec3( -20.0, -20.0, -30.0 );
        vec3 light_color = vec3( 0.5, 0.7, 1.0 );

        vec3 vl = normalize( light_pos - v );

        vec3 diffuse  = Kd * vec3( max( 0.0, dot( vl, n ) ) );
        vec3 specular = vec3( max( 0.0, dot( vl, ref ) ) );

        vec3 F = fresnel( Ks, normalize( vl - dir ), vl );
        specular = pow( specular, vec3( shininess ) );

        final += light_color * mix( diffuse, specular, F );
    }

    final += vec3(0.2, 0.1, 0.1) * fresnel( Ks, n, -dir );

    return final;
}


bool ray_vs_aabb(vec3 o, vec3 dir, vec3 bmin, vec3 bmax, inout vec2 e ) {
    vec3 a = ( bmin - o ) / dir;
    vec3 b = ( bmax - o ) / dir;

    vec3 s = min( a, b );
    vec3 t = max( a, b );

    e.x = max( max( s.x, s.y ), max( s.z, e.x ) );
    e.y = max( min( t.x, t.y ), max( t.z, e.y ) );

    return e.x < e.y;
}

// ray marching
bool ray_marching( vec3 o, vec3 dir, inout float depth, inout vec3 n ) {
    float t = 0.0;
    float d = 10000.0;
    float dt = 0.0;
    for ( int i = 0; i < 128; i++ ) {
        vec3 v = o + dir * t;
        d = dist_field( v );
        if ( d < 0.001 ) {
            break;
        }
        dt = min( abs(d), 0.1 );
        t += dt;
        if ( t > depth ) {
            break;
        }
    }

    if ( d >= 0.001 ) {
        return false;
    }

    t -= dt;
    for ( int i = 0; i < 4; i++ ) {
        dt *= 0.5;

        vec3 v = o + dir * ( t + dt );
        if ( dist_field( v ) >= 0.001 ) {
            t += dt;
        }
    }

    depth = t;
    n = normalize( gradient( o + dir * t ) );
    return true;

    return true;
}

// get ray direction
vec3 ray_dir( float fov, vec2 size, vec2 pos ) {
    vec2 xy = pos - size * 0.5;

    float cot_half_fov = tan( ( 90.0 - fov * 0.5 ) * DEG_TO_RAD );
    float z = size.y * 0.5 * cot_half_fov;

    return normalize( vec3( xy, -z ) );
}

// camera rotation : pitch, yaw
mat3 rotationXY( vec2 angle ) {
    vec2 c = cos( angle );
    vec2 s = sin( angle );

    return mat3(
                c.y      ,  0.0, -s.y,
                s.y * s.x,  c.x,  c.y * s.x,
                s.y * c.x, -s.x,  c.y * c.x
                );
}


void main()
{
    // default ray dir
    vec3 dir = ray_dir( 45.0, resolution, gl_FragCoord.xy );

    // default ray origin
    vec3 eye = vec3( 0.0, 0.0, 8.5 );

    // rotate camera
    mat3 rot = rotationXY( ( vec2(0, 0) - resolution * 0.5 ).yx * vec2( 0.01, -0.01 ) );
    dir = rot * dir;
    eye = rot * eye;

    // ray marching
    float depth = clip_far;
    vec3 n = vec3( 0.0 );
    if ( !ray_marching( eye, dir, depth, n ) ) {
        color = vec4(0.1, 0.1, 0.2, 1.0);
        return;
    }

    // shading
    vec3 pos = eye + dir * depth;

    vec3 newColor = shading( pos, n, dir, eye );
    color = vec4( pow( newColor, vec3(1.0/1.2) ), 1.0 );
}
