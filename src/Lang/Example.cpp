//#include <iostream>
//#include "parser.h"
//#include "lexer.h"

//using namespace std;

//int Example()
//{
//    const string data = R"(
//        arg X0(0, 4), Y0(-4, 4), Z0(-2, 2);
//        const ctx = 2;
//        var X=X0;
//        var Y=Y0;
//        var Z=Z0;
//        var S=ctx-X*X-Y*Y-Z*Z;
//        var A=3*(3-abs(X));
//        var B=3-abs(Y);
//        var C=3-abs(Z);
//        var K=A&B&C;
//        var SH=S&K;
//        var W=SH;
//        return W;
//    )";
//    try {
//        Lexer lexer;
//        lexer.SetText(data);
//        Parser parser(lexer);

//        Program program = parser.GetProgram();
//        auto args = program.GetArgs();

//        double xCoord = args[0].limits.first;
//        double step = 0.5;
//        while(xCoord < args[0].limits.second)
//        {
//            double yCoord = args[1].limits.first;
//            while(yCoord < args[1].limits.second)
//            {
//                double zCoord = args[2].limits.first;
//                while(zCoord < args[2].limits.second)
//                {
//                    cout<<program.Compute({{args[0].iter->second->name, xCoord},
//                                           {args[1].iter->second->name, yCoord},
//                                           {args[2].iter->second->name, zCoord}});
//                    cout<<endl;
//                    zCoord+= step;
//                }
//                yCoord += step;
//            }
//            xCoord += step;
//        }

//    }  catch (string& err) {
//        cerr<<"Exception: "<<err<<endl;
//        return -1;
//    }

//    return 0;
//}
