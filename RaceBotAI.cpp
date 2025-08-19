#include "RaceBotAI.h"

#include "ReparamUtils.h"

#include <glm/gtc/quaternion.hpp>

RaceBotAI::RaceBotAI()
{
    // Race trajectory of RR01 car on Night City track
    m_telemetry = {
        { 0.0065f, 8033.5f, 300.3f }, { 0.0187f, 8084.0f, 275.1f }, { 0.0428f, 8187.6f, 314.8f }, { 0.0663f, 8284.0f, 348.6f },
        { 0.0895f, 8373.7f, 376.2f }, { 0.1010f, 8415.2f, 399.3f }, { 0.1237f, 8499.1f, 475.3f }, { 0.1462f, 8583.3f, 572.6f },
        { 0.1686f, 8662.9f, 653.0f }, { 0.1909f, 8739.6f, 707.9f }, { 0.2132f, 8813.8f, 745.4f }, { 0.2354f, 8884.8f, 772.4f },
        { 0.2577f, 8952.4f, 792.0f }, { 0.2801f, 9017.2f, 806.9f }, { 0.3026f, 9079.5f, 819.5f }, { 0.3252f, 9139.2f, 828.9f },
        { 0.3480f, 9195.4f, 831.0f }, { 0.3711f, 9249.1f, 823.5f }, { 0.3945f, 9298.9f, 805.9f }, { 0.4182f, 9348.1f, 773.7f },
        { 0.4424f, 9395.6f, 741.4f }, { 0.4545f, 9264.6f, 728.0f }, { 0.4664f, 8910.3f, 719.0f }, { 0.4779f, 8533.5f, 716.5f },
        { 0.4891f, 8151.6f, 721.0f }, { 0.4997f, 7784.9f, 732.5f }, { 0.5100f, 7423.6f, 751.0f }, { 0.5199f, 7097.6f, 774.9f },
        { 0.5294f, 6810.2f, 801.1f }, { 0.5387f, 6635.0f, 825.4f }, { 0.5479f, 6504.8f, 844.7f }, { 0.5661f, 6432.7f, 863.4f },
        { 0.5845f, 6455.0f, 851.7f }, { 0.5940f, 6481.2f, 831.7f }, { 0.6035f, 6530.0f, 800.9f }, { 0.6231f, 6640.9f, 709.7f },
        { 0.6331f, 6712.7f, 652.6f }, { 0.6540f, 6861.0f, 528.3f }, { 0.6757f, 6917.6f, 402.9f }, { 0.6980f, 6911.8f, 279.6f },
        { 0.7207f, 6930.8f, 161.3f }, { 0.7440f, 6961.1f, 59.3f }, { 0.7677f, 6910.5f, -16.3f }, { 0.7911f, 6802.2f, -69.3f },
        { 0.8139f, 6638.9f, -108.0f }, { 0.8356f, 6414.9f, -136.9f }, { 0.8460f, 6268.9f, -146.0f }, { 0.8560f, 6043.4f, -146.5f },
        { 0.8654f, 5794.7f, -134.3f }, { 0.8742f, 5590.8f, -109.7f }, { 0.8826f, 5398.1f, -75.9f }, { 0.8905f, 5181.7f, -35.8f },
        { 0.8980f, 4968.7f, 8.5f }, { 0.9050f, 4818.4f, 53.6f }, { 0.9184f, 4714.0f, 133.8f }, { 0.9312f, 4704.8f, 192.5f },
        { 0.9436f, 4752.7f, 219.6f }, { 0.9558f, 4812.5f, 217.5f }, { 0.9678f, 4950.1f, 189.2f }, { 0.9798f, 5003.9f, 134.6f },
        { 0.9914f, 5044.3f, 46.8f }, { 0.9971f, 5056.7f, -12.0f }, { 1.0086f, 5076.0f, -155.3f }, { 1.0208f, 5135.4f, -297.9f },
        { 1.0273f, 5179.1f, -363.1f }, { 1.0410f, 5272.8f, -476.1f }, { 1.0556f, 5341.8f, -560.8f }, { 1.0619f, 5357.8f, -586.4f },
        { 1.0695f, 5373.8f, -608.8f }, { 1.0785f, 5392.3f, -624.3f }, { 1.0849f, 5405.5f, -628.9f }, { 1.1001f, 5490.0f, -621.3f },
        { 1.1152f, 5660.0f, -589.6f }, { 1.1300f, 5814.4f, -536.9f }, { 1.1444f, 5889.6f, -479.3f }, { 1.1584f, 6032.9f, -436.7f },
        { 1.1720f, 6174.4f, -418.9f }, { 1.1853f, 6303.5f, -433.5f }, { 1.1983f, 6469.0f, -486.6f }, { 1.2111f, 6646.5f, -540.0f },
        { 1.2175f, 6729.5f, -556.1f }, { 1.2302f, 6886.1f, -581.4f }, { 1.2427f, 7034.6f, -616.4f }, { 1.2550f, 7180.3f, -663.5f },
        { 1.2672f, 7321.5f, -701.8f }, { 1.2793f, 7456.2f, -714.5f }, { 1.2913f, 7584.4f, -705.0f }, { 1.3032f, 7705.5f, -686.3f },
        { 1.3150f, 7821.7f, -668.6f }, { 1.3267f, 7932.6f, -654.3f }, { 1.3384f, 8040.4f, -643.9f }, { 1.3500f, 8142.9f, -633.1f },
        { 1.3615f, 8241.1f, -622.0f }, { 1.3730f, 8335.1f, -615.7f }, { 1.3845f, 8424.5f, -617.2f }, { 1.3960f, 8511.3f, -627.0f },
        { 1.4074f, 8594.1f, -645.9f }, { 1.4188f, 8673.5f, -670.6f }, { 1.4302f, 8750.1f, -693.3f }, { 1.4416f, 8823.9f, -705.7f },
        { 1.4531f, 8894.0f, -702.8f }, { 1.4646f, 8962.0f, -684.4f }, { 1.4760f, 9026.9f, -653.7f }, { 1.4876f, 9089.3f, -614.8f },
        { 1.4991f, 9149.3f, -571.4f }, { 1.5000f, 9178.8f, -548.8f }, { 1.5000f, 9102.2f, -548.8f }, { 1.5007f, 9102.2f, -530.1f },
        { 1.5083f, 9141.5f, -514.8f }, { 1.5123f, 9137.7f, -499.4f }, { 1.5239f, 9197.2f, -469.1f }, { 1.5356f, 9252.8f, -440.7f },
        { 1.5474f, 9305.8f, -416.1f }, { 1.5592f, 9356.3f, -398.0f }, { 1.5712f, 9404.5f, -389.2f }, { 1.5833f, 9452.5f, -391.1f },
        { 1.5956f, 9498.1f, -402.2f }, { 1.6080f, 9541.2f, -418.8f }, { 1.6206f, 9583.0f, -438.5f }, { 1.6334f, 9623.1f, -462.6f },
        { 1.6463f, 9661.3f, -493.5f }, { 1.6595f, 9698.4f, -533.4f }, { 1.6730f, 9733.6f, -580.1f }, { 1.6867f, 9766.8f, -629.1f },
        { 1.7008f, 9796.6f, -678.4f }, { 1.7152f, 9825.0f, -718.5f }, { 1.7300f, 9850.6f, -742.0f }, { 1.7452f, 9871.8f, -745.8f },
        { 1.7530f, 9836.1f, -737.6f }, { 1.7609f, 9717.9f, -718.4f }, { 1.7688f, 9405.1f, -689.8f }, { 1.7764f, 9026.0f, -659.0f },
        { 1.7839f, 8649.2f, -630.7f }, { 1.7912f, 8299.6f, -605.6f }, { 1.7984f, 7987.9f, -583.3f }, { 1.8054f, 7737.7f, -559.9f },
        { 1.8124f, 7557.8f, -527.6f }, { 1.8194f, 7410.7f, -480.3f }, { 1.8264f, 7321.8f, -415.5f }, { 1.8335f, 7241.2f, -332.7f },
        { 1.8479f, 7196.6f, -125.3f }, { 1.8631f, 7219.2f, 96.8f }, { 1.8793f, 7263.0f, 293.1f }, { 1.8878f, 7303.8f, 389.8f },
        { 1.8967f, 7339.0f, 484.5f }, { 1.9061f, 7371.5f, 572.5f }, { 1.9158f, 7401.2f, 649.1f }, { 1.9259f, 7430.5f, 708.4f },
        { 1.9364f, 7459.3f, 745.0f }, { 1.9472f, 7488.2f, 752.9f }, { 1.9582f, 7517.3f, 727.1f }, { 1.9692f, 7545.1f, 664.3f },
        { 1.9800f, 7570.4f, 563.6f }, { 1.9904f, 7584.3f, 427.6f }, { 2.0003f, 7584.2f, 260.3f }, { 2.0098f, 7585.9f, 103.4f },
        { 2.0198f, 7613.5f, -8.9f }, { 2.0303f, 7665.8f, -78.4f }, { 2.0528f, 7779.6f, -153.1f }, { 2.0648f, 7811.8f, -195.3f },
        { 2.0773f, 7835.2f, -250.3f }, { 2.0904f, 7854.3f, -318.7f }, { 2.1041f, 7875.5f, -398.8f }, { 2.1337f, 7925.9f, -585.5f },
        { 2.1499f, 7983.9f, -680.7f }, { 2.1669f, 8036.9f, -762.5f }, { 2.1851f, 8084.0f, -821.8f }, { 2.2044f, 8130.5f, -855.5f },
        { 2.2246f, 8173.7f, -864.3f }, { 2.2457f, 8206.5f, -851.0f }, { 2.2674f, 8226.1f, -821.1f }, { 2.3122f, 8249.3f, -730.2f },
        { 2.3351f, 8258.8f, -675.3f }, { 2.3806f, 8273.9f, -558.9f }, { 2.4263f, 8293.2f, -424.3f }, { 2.4494f, 8304.5f, -336.3f },
        { 2.4961f, 8330.4f, -119.0f }, { 2.5199f, 8379.6f, -24.6f }, { 2.5682f, 8370.9f, 169.9f }, { 2.6174f, 8329.3f, 375.8f },
        { 2.6421f, 8323.8f, 466.0f }, { 2.6668f, 8328.6f, 542.8f }, { 2.6911f, 8333.6f, 607.3f }, { 2.7150f, 8331.7f, 661.5f },
        { 2.7381f, 8328.6f, 700.0f }, { 2.7604f, 8329.8f, 720.1f }, { 2.7816f, 8337.3f, 723.4f }, { 2.7985f, 8344.5f, 726.3f },
        { 2.8356f, 8353.8f, 685.5f }, { 2.8686f, 8364.7f, 635.9f }, { 2.8983f, 8379.7f, 588.3f }, { 2.9122f, 8387.0f, 557.7f },
        { 2.9254f, 8392.6f, 511.5f }, { 2.9381f, 8398.1f, 442.1f }, { 2.9621f, 8410.1f, 288.6f }, { 2.9735f, 8458.5f, 221.9f },
        { 2.9844f, 8488.2f, 142.4f }, { 2.9951f, 8517.3f, 33.8f }, { 3.0054f, 8565.5f, -96.2f }, { 3.0160f, 8606.9f, -207.1f },
        { 3.0269f, 8645.9f, -287.9f }, { 3.0380f, 8680.8f, -345.2f }, { 3.0608f, 8738.2f, -424.7f }, { 3.0845f, 8802.4f, -476.4f },
        { 3.1090f, 8810.7f, -510.9f }, { 3.1342f, 8792.4f, -567.0f }, { 3.1601f, 8768.6f, -653.5f }, { 3.1865f, 8759.0f, -765.5f },
        { 3.2136f, 8834.2f, -875.6f }, { 3.2274f, 8863.8f, -911.2f }, { 3.2414f, 8890.8f, -930.8f }, { 3.2699f, 8918.7f, -939.6f },
        { 3.2988f, 8922.6f, -939.7f }, { 3.3281f, 8960.8f, -940.0f }, { 3.3578f, 9003.2f, -920.9f }, { 3.3878f, 9029.1f, -869.4f },
        { 3.4179f, 9020.0f, -805.3f }, { 3.4481f, 9011.0f, -743.8f }, { 3.4783f, 9009.1f, -687.0f }, { 3.5085f, 9013.0f, -629.9f },
        { 3.5387f, 9000.1f, -570.2f }, { 3.5686f, 8979.1f, -510.4f }, { 3.5984f, 8955.4f, -452.5f }, { 3.6278f, 8934.1f, -396.0f },
        { 3.6569f, 8917.0f, -340.2f }, { 3.6854f, 8902.7f, -285.0f }, { 3.7135f, 8889.2f, -231.3f }, { 3.7409f, 8875.3f, -181.6f },
        { 3.7676f, 8862.5f, -139.5f }, { 3.7937f, 8856.0f, -109.8f }, { 3.8190f, 8854.0f, -96.4f }, { 3.8435f, 8843.1f, -105.1f },
        { 3.8673f, 8831.6f, -143.9f }, { 3.8903f, 8885.6f, -210.1f }, { 3.9015f, 8918.4f, -238.9f }, { 3.9237f, 8975.5f, -263.6f },
        { 3.9451f, 9020.9f, -266.3f }, { 3.9659f, 9054.3f, -280.5f }, { 3.9860f, 9104.6f, -316.0f }, { 4.0056f, 9161.9f, -347.4f },
        { 4.0253f, 9215.8f, -378.1f }, { 4.0457f, 9266.7f, -425.4f }, { 4.0667f, 9320.7f, -469.5f }, { 4.0884f, 9370.9f, -482.8f },
        { 4.1109f, 9413.4f, -477.4f }, { 4.1340f, 9457.1f, -481.4f }, { 4.1580f, 9495.5f, -495.5f }, { 4.1827f, 9534.9f, -518.0f },
        { 4.2084f, 9574.4f, -544.2f }, { 4.2349f, 9613.2f, -554.0f }, { 4.2624f, 9645.7f, -538.0f }, { 4.2909f, 9677.4f, -513.0f },
        { 4.3204f, 9708.2f, -475.2f }, { 4.3509f, 9732.5f, -422.5f }, { 4.3823f, 9755.5f, -360.2f }, { 4.4148f, 9761.1f, -298.2f },
        { 4.4481f, 9764.2f, -256.9f }, { 4.4823f, 9771.2f, -235.9f }, { 4.5173f, 9781.4f, -224.7f }, { 4.5529f, 9789.1f, -213.8f },
        { 4.5891f, 9800.2f, -198.9f }, { 4.6257f, 9815.8f, -169.7f }, { 4.6624f, 9829.9f, -118.2f }, { 4.6992f, 9843.5f, -47.0f },
        { 4.7359f, 9856.6f, 39.9f }, { 4.7722f, 9866.9f, 136.9f }, { 4.8082f, 9879.0f, 236.5f }, { 4.8437f, 9889.4f, 331.8f },
        { 4.8787f, 9899.6f, 415.9f }, { 4.9132f, 9895.8f, 475.8f }, { 4.9302f, 9885.1f, 487.1f }, { 4.9471f, 9873.5f, 480.7f },
        { 4.9638f, 9864.0f, 454.3f }, { 4.9803f, 9853.5f, 407.3f }, { 4.9967f, 9847.6f, 339.0f }, { 5.0298f, 9855.8f, 156.4f },
        { 5.0646f, 9879.2f, -32.9f }, { 5.1010f, 9896.1f, -188.3f }, { 5.1391f, 9899.2f, -332.8f }, { 5.1786f, 9909.1f, -482.8f },
        { 5.2194f, 9934.8f, -619.5f }, { 5.2402f, 9946.5f, -667.5f }, { 5.2614f, 9958.9f, -699.8f }, { 5.2826f, 9970.0f, -718.5f },
        { 5.3255f, 9990.9f, -724.0f }, { 5.3688f, 10009.3f, -700.5f }, { 5.4120f, 10024.7f, -666.7f }, { 5.4552f, 10039.9f, -632.4f },
        { 5.4982f, 10052.8f, -595.3f }, { 5.5409f, 10063.1f, -569.9f }, { 5.5831f, 10072.2f, -574.9f }, { 5.6248f, 10078.2f, -622.0f },
        { 5.6657f, 10067.3f, -680.0f }, { 5.7058f, 10056.4f, -707.2f }, { 5.7447f, 10048.8f, -707.8f }, { 5.7639f, 9997.5f, -701.6f },
        { 5.7823f, 9747.1f, -692.6f }, { 5.8012f, 9343.3f, -685.9f }, { 5.8166f, 8982.8f, -686.5f }, { 5.8323f, 8599.9f, -694.8f },
        { 5.8471f, 8264.3f, -710.3f }, { 5.8612f, 7995.7f, -728.9f }, { 5.8747f, 7779.0f, -744.9f }, { 5.8877f, 7660.5f, -754.1f },
        { 5.9127f, 7597.7f, -745.6f }, { 5.9369f, 7568.1f, -703.8f }, { 5.9487f, 7565.8f, -667.4f }, { 5.9603f, 7576.9f, -617.1f },
        { 5.9717f, 7597.7f, -552.1f }, { 5.9831f, 7616.9f, -470.3f }, { 5.9941f, 7632.1f, -370.4f }, { 6.0165f, 7675.2f, -125.2f },
        { 6.0409f, 7722.7f, 137.4f }, { 6.0541f, 7748.4f, 268.3f }, { 6.0680f, 7771.5f, 393.2f }, { 6.0828f, 7793.0f, 507.3f },
        { 6.0984f, 7815.8f, 605.8f }, { 6.1148f, 7814.4f, 683.6f }, { 6.1318f, 7749.0f, 737.3f }, { 6.1499f, 7668.0f, 765.1f },
        { 6.1677f, 7587.2f, 767.3f }, { 6.1853f, 7510.9f, 746.4f }, { 6.2028f, 7433.9f, 706.0f }, { 6.2198f, 7352.2f, 651.1f },
        { 6.2365f, 7274.7f, 586.4f }, { 6.2525f, 7194.4f, 517.1f }, { 6.2681f, 7113.8f, 446.9f }, { 6.2832f, 7035.0f, 379.9f },
        { 6.2979f, 6956.3f, 318.5f }, { 6.3097f, 6892.2f, 257.1f }, { 6.3259f, 6808.9f, 198.0f }, { 6.3394f, 6744.2f, 139.5f },
        { 6.3505f, 6692.7f, 77.4f }, { 6.3636f, 6639.7f, 11.7f }, { 6.3764f, 6604.9f, -54.8f }, { 6.3890f, 6588.0f, -118.5f },
        { 6.4016f, 6595.4f, -175.8f }, { 6.4142f, 6634.6f, -223.0f }, { 6.4267f, 6693.6f, -256.1f }, { 6.4393f, 6754.8f, -268.0f },
        { 6.4518f, 6818.5f, -243.0f }, { 6.4774f, 6980.0f, -166.7f }, { 6.5035f, 7129.7f, -74.5f }, { 6.5303f, 7270.7f, 0.9f },
        { 6.5577f, 7294.4f, 48.0f }, { 6.5712f, 7023.7f, 56.6f }, { 6.5843f, 6691.8f, 63.0f }, { 6.5969f, 6357.1f, 73.3f },
        { 6.6088f, 6033.0f, 88.0f }, { 6.6203f, 5725.2f, 107.0f }, { 6.6313f, 5424.2f, 129.8f }, { 6.6418f, 5153.8f, 155.2f },
        { 6.6519f, 4932.5f, 179.9f }, { 6.6618f, 4724.1f, 201.8f }, { 6.6713f, 4517.0f, 220.4f }, { 6.6805f, 4331.8f, 234.6f },
        { 6.6895f, 4271.4f, 239.9f }, { 6.6987f, 4253.1f, 233.7f }, { 6.7171f, 4234.6f, 190.6f }, { 6.7361f, 4219.2f, 113.8f },
        { 6.7556f, 4184.3f, 1.3f }, { 6.7756f, 4136.8f, -147.1f }, { 6.7966f, 4114.8f, -317.3f }, { 6.8186f, 4115.8f, -472.6f },
        { 6.8433f, 4110.1f, -587.8f }, { 6.8690f, 4064.6f, -667.7f }, { 6.8822f, 4030.7f, -696.2f }, { 6.8955f, 3990.1f, -715.8f },
        { 6.9085f, 3944.0f, -726.3f }, { 6.9338f, 3851.8f, -713.8f }, { 6.9569f, 3755.8f, -675.9f }, { 6.9779f, 3721.3f, -631.0f },
        { 6.9971f, 3789.8f, -593.3f }, { 7.0161f, 3883.8f, -569.7f }, { 7.0368f, 3895.0f, -562.8f }, { 7.0594f, 3893.0f, -565.7f },
        { 7.0841f, 3972.6f, -562.7f }, { 7.1106f, 3968.0f, -536.0f }, { 7.1375f, 3865.9f, -489.7f }, { 7.1508f, 3786.6f, -446.8f },
        { 7.1765f, 3659.3f, -376.9f }, { 7.2010f, 3615.6f, -304.0f }, { 7.2248f, 3708.5f, -241.9f }, { 7.2367f, 3797.2f, -188.6f },
        { 7.2609f, 4004.2f, -136.4f }, { 7.2731f, 4087.3f, -87.8f }, { 7.2977f, 4265.1f, -45.2f }, { 7.3100f, 4350.4f, -9.0f },
        { 7.3223f, 4449.0f, 22.7f }, { 7.3348f, 4568.3f, 52.8f }, { 7.3473f, 4718.7f, 82.7f }, { 7.3599f, 4877.4f, 112.1f },
        { 7.3726f, 5034.0f, 139.0f }, { 7.3985f, 5300.8f, 160.0f }, { 7.4116f, 5441.5f, 171.1f }, { 7.4247f, 5572.5f, 168.7f },
        { 7.4380f, 5699.9f, 151.1f }, { 7.4513f, 5830.8f, 118.8f }, { 7.4646f, 5961.7f, 74.5f }, { 7.4916f, 6184.5f, 22.5f },
        { 7.5191f, 6390.8f, -32.2f }, { 7.5470f, 6574.3f, -84.3f }, { 7.5611f, 6667.4f, -129.4f }, { 7.5754f, 6757.4f, -164.1f },
        { 7.5897f, 6839.9f, -185.7f }, { 7.6042f, 6916.9f, -191.5f }, { 7.6335f, 7052.7f, -178.6f }, { 7.6633f, 7177.1f, -144.5f },
        { 7.6937f, 7283.4f, -88.6f }, { 7.7246f, 7373.2f, -13.2f }, { 7.7558f, 7457.2f, 75.1f }, { 7.7875f, 7537.8f, 166.1f },
        { 7.8192f, 7615.0f, 247.1f }, { 7.8509f, 7687.5f, 307.1f }, { 7.8819f, 7754.9f, 339.5f }, { 7.9120f, 7820.3f, 344.0f },
        { 7.9408f, 7881.1f, 326.7f }, { 7.9682f, 7941.0f, 300.4f }, { 7.9942f, 7996.6f, 300.3f }
    };

    float m_vMin = INFINITY;
    float m_vMax = 0.f;
    for (int i = 0; i < m_telemetry.size(); ++i) {
        float v = m_telemetry[i].speed;
        if (v > m_vMax)
            m_vMax = v;
        if (v < m_vMin)
            m_vMin = v;
    }

    // Increase desired high speed to avoid throttle release at high speed
    for (auto& t : m_telemetry) {
        float vMid = (m_vMin + m_vMax) / 2;
        float normalizedVel = glm::clamp(normalizeRange(m_vMin, vMid, t.speed), 0.f, 2.f);
        normalizedVel = pow(normalizedVel, 1.2f) * 1.1f;
        t.speed = glm::mix(m_vMin, m_vMax, normalizedVel / 2);
    }
}

#define FROM_KPH(x) ((x) / 0.036f)

constexpr bool reverse = false;
constexpr float roadHalfWidth = 800;
constexpr float steeringScale = 7.f;
constexpr float minRadius = 20'00;
constexpr float gravityUnrealUnits = 980;
constexpr float botReceiveUpdateTimeDelta = 0.1f;

template <int precision>
void print(const char* str, const glm::dvec3& v)
{
    printf(str);
    static_assert(precision < 5, "too precise");
    // clang-format off
    if constexpr        (precision == 0) { printf("%5.0f, %5.0f, %5.0f", v.x, v.y, v.z);
    } else if constexpr (precision == 1) { printf("%5.1f, %5.1f, %5.1f", v.x, v.y, v.z);
    } else if constexpr (precision == 2) { printf("%5.2f, %5.2f, %5.2f", v.x, v.y, v.z);
    } else if constexpr (precision == 3) { printf("%5.3f, %5.3f, %5.3f", v.x, v.y, v.z);
    } else if constexpr (precision == 4) { printf("%5.4f, %5.4f, %5.4f", v.x, v.y, v.z); }
    // clang-format on
}

inline float getForwardOffsetAtSpeed(float speed)
{

    float speed2 = speed * speed;
    float maxAccAtSpeed = (2.0f + 4.0f * speed2 / powf(100'00.f, 2)) * gravityUnrealUnits;
    return glm::max(minRadius, speed2 / maxAccAtSpeed / 2.4f);
}

CarInput RaceBotAI::Calculate(const CarData& carData)
{
    // unreal's pitch, yaw, roll  is  glm's roll, pitch, yaw
    glm::quat carRotation_WS = glm::quat(glm::radians(glm::vec3(carData.roll, carData.pitch, carData.yaw)));
    glm::dvec3 carPosition_WS(carData.pos[0], carData.pos[1], carData.pos[2]);

    float reverseSign = reverse ? -1 : 1;
    const float dataRetrieveOffset = getForwardOffsetAtSpeed(carData.speed) * reverseSign;

    const float carSplineKey = (float)m_spline.GetKeyClosestToPosition(carPosition_WS);
    // m_followSplineIndex = static_cast<int>(carSplineKey);

    const double trackLength = m_spline.GetLength();
    double currentDist = m_spline.KeyToDistance(carSplineKey);

    double targetDist = fmod(currentDist + dataRetrieveOffset, trackLength);
    if (targetDist < 0.f) // if reverse
        targetDist += trackLength;

    // data retrieve target position if offset forward by dataRetrieveOffset
    const double targetSplineKey = m_spline.DistanceToKey(targetDist);

    auto siTarget = m_spline.GetInterpAtKey(targetSplineKey);

    glm::dvec3 splinePosTarget_WS, splineForwardTarget_WS, splineRightTarget_WS, splineUpTarget_WS;
    float desiredOffsetY, desiredVel;
    {
        // retrieve slice frame at target
        splinePosTarget_WS = siTarget.getPos();
        siTarget.getFrame(splineForwardTarget_WS, splineRightTarget_WS, splineUpTarget_WS);

        // retrieve telemetry at target
        auto telemetryInterp = GetInterpData<TelemetryPoint, float, &TelemetryPoint::splineKey>(
            m_telemetry, (float)targetSplineKey);
        desiredOffsetY = telemetryInterp.GetValue<&TelemetryPoint::offsetY>();
        desiredVel = telemetryInterp.GetValue<&TelemetryPoint::speed>();
    }

    // throttle > 0, braking < 0, depends on diff between current and desired speed
    float throttleBraking = remapRangeClamped(0.95f * desiredVel, 1.05f * desiredVel, 1.f, -1.f, carData.speed);

    glm::dvec3 splinePosCar_WS, splineForwardCar_WS, splineRightCar_WS, splineUpCar_WS;
    {
        auto siCar = m_spline.GetInterpAtKey(carSplineKey);
        siCar.getFrame(splineForwardCar_WS, splineRightCar_WS, splineUpCar_WS);
        splinePosCar_WS = siCar.getPos();
    }

    float carOffsetY = (float)glm::dot(carPosition_WS - splinePosCar_WS, splineRightCar_WS);

    // printf("carOffsetY: %f, desiredOffsetY: %f\n", carOffsetY, desiredOffsetY);

    if (throttleBraking > 0) { // decrease throttle
        throttleBraking *= remapRangeClamped(0.f, FROM_KPH(100), 0.6f, 1.f, carData.speed); // at low speed
        throttleBraking *= remapRangeClamped(roadHalfWidth - 100.f, roadHalfWidth, 1.f, 0.5f, fabs(carOffsetY)); // if far close to road bounds
    }

    float actualSteeringScale = steeringScale;
    if (m_startFactor > 0) {
        m_startFactor -= botReceiveUpdateTimeDelta / 5.f;
        throttleBraking *= (1 - m_startFactor) * 0.5f + 0.5f;
        actualSteeringScale *= pow(1 - m_startFactor, 0.5f);
    }

    glm::dvec3 targetPointPos_WS = splinePosTarget_WS + splineRightTarget_WS * (double)desiredOffsetY;

    glm::dmat3 carRotationMatrix_WS = glm::mat3_cast(carRotation_WS);
    glm::dvec3 carForward_WS = carRotationMatrix_WS[0];
    glm::dvec3 carRight_WS = carRotationMatrix_WS[1];

    glm::dvec3 dirToFollow_WS = normalize(targetPointPos_WS - carPosition_WS);
    float steering = (float)glm::dot(dirToFollow_WS, carRight_WS) * actualSteeringScale;

    bool carLooksOppositeFollow = glm::dot(dirToFollow_WS, carForward_WS) < 0.f;
    if (carLooksOppositeFollow) // turn around
        steering = glm::sign(steering);

    CarInput carInput;
    carInput.steering = steering;
    carInput.throttle = glm::clamp(throttleBraking, 0.f, 1.f);
    carInput.braking = glm::clamp(-throttleBraking, 0.f, 1.f);
    return carInput;
}
