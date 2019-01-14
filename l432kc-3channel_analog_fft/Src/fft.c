//
// From: Numerical Recipes in C, 2nd Ed. 1997
//
// four1.c on p.507
// realft.c on p.513
#include <math.h>
#include "main.h"
#include "fft.h"

//-------------------------------------------------------
//
// FFT - BEGIN
//
//-------------------------------------------------------

#ifdef SINE_WINDOW
// sine window:
//    w[m] = sin(pi/1024 * m) / WG, m=0,1,..1023
// where the window gain is:
//    WG   = Integral(t,0,T)[ sin(pi t / T) ] = 2/pi
const float window_sine_unit_gain_1024[] = {
  0.000000000,0.004819135,0.009638225,0.014457224,0.019276087,0.024094769,0.028913224,0.033731406,
  0.038549272,0.043366774,0.048183868,0.053000509,0.057816651,0.062632248,0.067447256,0.072261630,
  0.077075323,0.081888290,0.086700487,0.091511868,0.096322387,0.101132000,0.105940661,0.110748325,
  0.115554946,0.120360480,0.125164881,0.129968104,0.134770103,0.139570834,0.144370251,0.149168310,
  0.153964964,0.158760169,0.163553880,0.168346051,0.173136638,0.177925596,0.182712878,0.187498441,
  0.192282239,0.197064227,0.201844360,0.206622594,0.211398883,0.216173181,0.220945446,0.225715630,
  0.230483690,0.235249581,0.240013257,0.244774675,0.249533788,0.254290553,0.259044924,0.263796857,
  0.268546307,0.273293229,0.278037579,0.282779312,0.287518384,0.292254749,0.296988363,0.301719182,
  0.306447161,0.311172256,0.315894422,0.320613614,0.325329789,0.330042902,0.334752908,0.339459763,
  0.344163423,0.348863844,0.353560981,0.358254790,0.362945228,0.367632249,0.372315810,0.376995866,
  0.381672374,0.386345290,0.391014569,0.395680167,0.400342042,0.405000148,0.409654442,0.414304881,
  0.418951420,0.423594015,0.428232624,0.432867202,0.437497705,0.442124091,0.446746315,0.451364334,
  0.455978105,0.460587584,0.465192727,0.469793493,0.474389836,0.478981714,0.483569084,0.488151902,
  0.492730126,0.497303711,0.501872616,0.506436798,0.510996212,0.515550817,0.520100569,0.524645426,
  0.529185344,0.533720282,0.538250196,0.542775044,0.547294783,0.551809371,0.556318765,0.560822922,
  0.565321801,0.569815359,0.574303554,0.578786343,0.583263684,0.587735536,0.592201855,0.596662601,
  0.601117730,0.605567201,0.610010973,0.614449003,0.618881249,0.623307671,0.627728225,0.632142871,
  0.636551568,0.640954272,0.645350944,0.649741541,0.654126023,0.658504348,0.662876475,0.667242363,
  0.671601970,0.675955256,0.680302180,0.684642700,0.688976776,0.693304368,0.697625433,0.701939933,
  0.706247825,0.710549070,0.714843627,0.719131456,0.723412516,0.727686766,0.731954168,0.736214680,
  0.740468263,0.744714876,0.748954479,0.753187033,0.757412498,0.761630834,0.765842001,0.770045959,
  0.774242670,0.778432093,0.782614189,0.786788919,0.790956244,0.795116123,0.799268519,0.803413392,
  0.807550702,0.811680412,0.815802482,0.819916873,0.824023547,0.828122464,0.832213588,0.836296878,
  0.840372296,0.844439805,0.848499365,0.852550939,0.856594489,0.860629976,0.864657362,0.868676610,
  0.872687681,0.876690539,0.880685144,0.884671461,0.888649450,0.892619076,0.896580299,0.900533084,
  0.904477392,0.908413187,0.912340432,0.916259089,0.920169123,0.924070495,0.927963170,0.931847110,
  0.935722279,0.939588641,0.943446160,0.947294798,0.951134520,0.954965289,0.958787070,0.962599827,
  0.966403523,0.970198123,0.973983591,0.977759891,0.981526989,0.985284848,0.989033433,0.992772709,
  0.996502641,1.000223193,1.003934330,1.007636019,1.011328223,1.015010908,1.018684039,1.022347582,
  1.026001502,1.029645766,1.033280338,1.036905184,1.040520270,1.044125563,1.047721028,1.051306631,
  1.054882339,1.058448119,1.062003935,1.065549756,1.069085547,1.072611276,1.076126909,1.079632413,
  1.083127755,1.086612902,1.090087822,1.093552481,1.097006847,1.100450888,1.103884571,1.107307864,
  1.110720735,1.114123150,1.117515080,1.120896491,1.124267351,1.127627630,1.130977295,1.134316314,
  1.137644658,1.140962293,1.144269189,1.147565314,1.150850639,1.154125131,1.157388760,1.160641495,
  1.163883306,1.167114162,1.170334033,1.173542888,1.176740697,1.179927430,1.183103057,1.186267549,
  1.189420875,1.192563005,1.195693911,1.198813562,1.201921930,1.205018985,1.208104698,1.211179039,
  1.214241981,1.217293493,1.220333548,1.223362117,1.226379171,1.229384682,1.232378621,1.235360961,
  1.238331673,1.241290729,1.244238102,1.247173764,1.250097686,1.253009843,1.255910205,1.258798747,
  1.261675440,1.264540258,1.267393173,1.270234160,1.273063190,1.275880238,1.278685277,1.281478280,
  1.284259221,1.287028075,1.289784814,1.292529414,1.295261848,1.297982090,1.300690116,1.303385898,
  1.306069413,1.308740634,1.311399537,1.314046097,1.316680289,1.319302087,1.321911467,1.324508406,
  1.327092877,1.329664857,1.332224322,1.334771248,1.337305610,1.339827385,1.342336549,1.344833079,
  1.347316950,1.349788140,1.352246625,1.354692382,1.357125389,1.359545622,1.361953058,1.364347675,
  1.366729450,1.369098361,1.371454385,1.373797501,1.376127686,1.378444918,1.380749176,1.383040438,
  1.385318683,1.387583888,1.389836032,1.392075095,1.394301055,1.396513892,1.398713584,1.400900110,
  1.403073451,1.405233586,1.407380494,1.409514155,1.411634550,1.413741657,1.415835458,1.417915933,
  1.419983061,1.422036824,1.424077203,1.426104177,1.428117728,1.430117838,1.432104486,1.434077655,
  1.436037326,1.437983481,1.439916100,1.441835167,1.443740662,1.445632569,1.447510868,1.449375543,
  1.451226576,1.453063950,1.454887646,1.456697649,1.458493941,1.460276504,1.462045324,1.463800381,
  1.465541661,1.467269147,1.468982822,1.470682671,1.472368677,1.474040824,1.475699098,1.477343481,
  1.478973959,1.480590517,1.482193138,1.483781809,1.485356513,1.486917237,1.488463966,1.489996684,
  1.491515378,1.493020034,1.494510636,1.495987172,1.497449626,1.498897987,1.500332239,1.501752369,
  1.503158364,1.504550211,1.505927897,1.507291408,1.508640732,1.509975856,1.511296768,1.512603454,
  1.513895904,1.515174104,1.516438043,1.517687708,1.518923089,1.520144173,1.521350948,1.522543404,
  1.523721529,1.524885313,1.526034743,1.527169810,1.528290503,1.529396811,1.530488724,1.531566231,
  1.532629322,1.533677988,1.534712218,1.535732002,1.536737332,1.537728198,1.538704590,1.539666499,
  1.540613916,1.541546832,1.542465239,1.543369127,1.544258489,1.545133315,1.545993598,1.546839329,
  1.547670502,1.548487106,1.549289136,1.550076584,1.550849441,1.551607701,1.552351357,1.553080402,
  1.553794828,1.554494630,1.555179800,1.555850332,1.556506220,1.557147457,1.557774038,1.558385957,
  1.558983207,1.559565784,1.560133682,1.560686895,1.561225418,1.561749246,1.562258374,1.562752798,
  1.563232513,1.563697514,1.564147797,1.564583357,1.565004192,1.565410295,1.565801665,1.566178296,
  1.566540186,1.566887331,1.567219728,1.567537374,1.567840265,1.568128400,1.568401774,1.568660387,
  1.568904234,1.569133314,1.569347625,1.569547165,1.569731931,1.569901923,1.570057138,1.570197575,
  1.570323232,1.570434110,1.570530205,1.570611519,1.570678049,1.570729795,1.570766757,1.570788934,
  1.570796327,1.570788934,1.570766757,1.570729795,1.570678049,1.570611519,1.570530205,1.570434110,
  1.570323232,1.570197575,1.570057138,1.569901923,1.569731931,1.569547165,1.569347625,1.569133314,
  1.568904234,1.568660387,1.568401774,1.568128400,1.567840265,1.567537374,1.567219728,1.566887331,
  1.566540186,1.566178296,1.565801665,1.565410295,1.565004192,1.564583357,1.564147797,1.563697514,
  1.563232513,1.562752798,1.562258374,1.561749246,1.561225418,1.560686895,1.560133682,1.559565784,
  1.558983207,1.558385957,1.557774038,1.557147457,1.556506220,1.555850332,1.555179800,1.554494630,
  1.553794828,1.553080402,1.552351357,1.551607701,1.550849441,1.550076584,1.549289136,1.548487106,
  1.547670502,1.546839329,1.545993598,1.545133315,1.544258489,1.543369127,1.542465239,1.541546832,
  1.540613916,1.539666499,1.538704590,1.537728198,1.536737332,1.535732002,1.534712218,1.533677988,
  1.532629322,1.531566231,1.530488724,1.529396811,1.528290503,1.527169810,1.526034743,1.524885313,
  1.523721529,1.522543404,1.521350948,1.520144173,1.518923089,1.517687708,1.516438043,1.515174104,
  1.513895904,1.512603454,1.511296768,1.509975856,1.508640732,1.507291408,1.505927897,1.504550211,
  1.503158364,1.501752369,1.500332239,1.498897987,1.497449626,1.495987172,1.494510636,1.493020034,
  1.491515378,1.489996684,1.488463966,1.486917237,1.485356513,1.483781809,1.482193138,1.480590517,
  1.478973959,1.477343481,1.475699098,1.474040824,1.472368677,1.470682671,1.468982822,1.467269147,
  1.465541661,1.463800381,1.462045324,1.460276504,1.458493941,1.456697649,1.454887646,1.453063950,
  1.451226576,1.449375543,1.447510868,1.445632569,1.443740662,1.441835167,1.439916100,1.437983481,
  1.436037326,1.434077655,1.432104486,1.430117838,1.428117728,1.426104177,1.424077203,1.422036824,
  1.419983061,1.417915933,1.415835458,1.413741657,1.411634550,1.409514155,1.407380494,1.405233586,
  1.403073451,1.400900110,1.398713584,1.396513892,1.394301055,1.392075095,1.389836032,1.387583888,
  1.385318683,1.383040438,1.380749176,1.378444918,1.376127686,1.373797501,1.371454385,1.369098361,
  1.366729450,1.364347675,1.361953058,1.359545622,1.357125389,1.354692382,1.352246625,1.349788140,
  1.347316950,1.344833079,1.342336549,1.339827385,1.337305610,1.334771248,1.332224322,1.329664857,
  1.327092877,1.324508406,1.321911467,1.319302087,1.316680289,1.314046097,1.311399537,1.308740634,
  1.306069413,1.303385898,1.300690116,1.297982090,1.295261848,1.292529414,1.289784814,1.287028075,
  1.284259221,1.281478280,1.278685277,1.275880238,1.273063190,1.270234160,1.267393173,1.264540258,
  1.261675440,1.258798747,1.255910205,1.253009843,1.250097686,1.247173764,1.244238102,1.241290729,
  1.238331673,1.235360961,1.232378621,1.229384682,1.226379171,1.223362117,1.220333548,1.217293493,
  1.214241981,1.211179039,1.208104698,1.205018985,1.201921930,1.198813562,1.195693911,1.192563005,
  1.189420875,1.186267549,1.183103057,1.179927430,1.176740697,1.173542888,1.170334033,1.167114162,
  1.163883306,1.160641495,1.157388760,1.154125131,1.150850639,1.147565314,1.144269189,1.140962293,
  1.137644658,1.134316314,1.130977295,1.127627630,1.124267351,1.120896491,1.117515080,1.114123150,
  1.110720735,1.107307864,1.103884571,1.100450888,1.097006847,1.093552481,1.090087822,1.086612902,
  1.083127755,1.079632413,1.076126909,1.072611276,1.069085547,1.065549756,1.062003935,1.058448119,
  1.054882339,1.051306631,1.047721028,1.044125563,1.040520270,1.036905184,1.033280338,1.029645766,
  1.026001502,1.022347582,1.018684039,1.015010908,1.011328223,1.007636019,1.003934330,1.000223193,
  0.996502641,0.992772709,0.989033433,0.985284848,0.981526989,0.977759891,0.973983591,0.970198123,
  0.966403523,0.962599827,0.958787070,0.954965289,0.951134520,0.947294798,0.943446160,0.939588641,
  0.935722279,0.931847110,0.927963170,0.924070495,0.920169123,0.916259089,0.912340432,0.908413187,
  0.904477392,0.900533084,0.896580299,0.892619076,0.888649450,0.884671461,0.880685144,0.876690539,
  0.872687681,0.868676610,0.864657362,0.860629976,0.856594489,0.852550939,0.848499365,0.844439805,
  0.840372296,0.836296878,0.832213588,0.828122464,0.824023547,0.819916873,0.815802482,0.811680412,
  0.807550702,0.803413392,0.799268519,0.795116123,0.790956244,0.786788919,0.782614189,0.778432093,
  0.774242670,0.770045959,0.765842001,0.761630834,0.757412498,0.753187033,0.748954479,0.744714876,
  0.740468263,0.736214680,0.731954168,0.727686766,0.723412516,0.719131456,0.714843627,0.710549070,
  0.706247825,0.701939933,0.697625433,0.693304368,0.688976776,0.684642700,0.680302180,0.675955256,
  0.671601970,0.667242363,0.662876475,0.658504348,0.654126023,0.649741541,0.645350944,0.640954272,
  0.636551568,0.632142871,0.627728225,0.623307671,0.618881249,0.614449003,0.610010973,0.605567201,
  0.601117730,0.596662601,0.592201855,0.587735536,0.583263684,0.578786343,0.574303554,0.569815359,
  0.565321801,0.560822922,0.556318765,0.551809371,0.547294783,0.542775044,0.538250196,0.533720282,
  0.529185344,0.524645426,0.520100569,0.515550817,0.510996212,0.506436798,0.501872616,0.497303711,
  0.492730126,0.488151902,0.483569084,0.478981714,0.474389836,0.469793493,0.465192727,0.460587584,
  0.455978105,0.451364334,0.446746315,0.442124091,0.437497705,0.432867202,0.428232624,0.423594015,
  0.418951420,0.414304881,0.409654442,0.405000148,0.400342042,0.395680167,0.391014569,0.386345290,
  0.381672374,0.376995866,0.372315810,0.367632249,0.362945228,0.358254790,0.353560981,0.348863844,
  0.344163423,0.339459763,0.334752908,0.330042902,0.325329789,0.320613614,0.315894422,0.311172256,
  0.306447161,0.301719182,0.296988363,0.292254749,0.287518384,0.282779312,0.278037579,0.273293229,
  0.268546307,0.263796857,0.259044924,0.254290553,0.249533788,0.244774675,0.240013257,0.235249581,
  0.230483690,0.225715630,0.220945446,0.216173181,0.211398883,0.206622594,0.201844360,0.197064227,
  0.192282239,0.187498441,0.182712878,0.177925596,0.173136638,0.168346051,0.163553880,0.158760169,
  0.153964964,0.149168310,0.144370251,0.139570834,0.134770103,0.129968104,0.125164881,0.120360480,
  0.115554946,0.110748325,0.105940661,0.101132000,0.096322387,0.091511868,0.086700487,0.081888290,
  0.077075323,0.072261630,0.067447256,0.062632248,0.057816651,0.053000509,0.048183868,0.043366774,
  0.038549272,0.033731406,0.028913224,0.024094769,0.019276087,0.014457224,0.009638225,0.004819135
};
#endif

// unit gain bartlett window:
//    w[m] =
//             4 * m / N, for m=0,1,..512
//             4 * (N-m) / N, for m=513..1023
#ifdef BARTLETT_WINDOW
const float window_bartlett_unit_gain_1024[] = {
  0.000000000,0.003906250,0.007812500,0.011718750,0.015625000,0.019531250,0.023437500,0.027343750,
  0.031250000,0.035156250,0.039062500,0.042968750,0.046875000,0.050781250,0.054687500,0.058593750,
  0.062500000,0.066406250,0.070312500,0.074218750,0.078125000,0.082031250,0.085937500,0.089843750,
  0.093750000,0.097656250,0.101562500,0.105468750,0.109375000,0.113281250,0.117187500,0.121093750,
  0.125000000,0.128906250,0.132812500,0.136718750,0.140625000,0.144531250,0.148437500,0.152343750,
  0.156250000,0.160156250,0.164062500,0.167968750,0.171875000,0.175781250,0.179687500,0.183593750,
  0.187500000,0.191406250,0.195312500,0.199218750,0.203125000,0.207031250,0.210937500,0.214843750,
  0.218750000,0.222656250,0.226562500,0.230468750,0.234375000,0.238281250,0.242187500,0.246093750,
  0.250000000,0.253906250,0.257812500,0.261718750,0.265625000,0.269531250,0.273437500,0.277343750,
  0.281250000,0.285156250,0.289062500,0.292968750,0.296875000,0.300781250,0.304687500,0.308593750,
  0.312500000,0.316406250,0.320312500,0.324218750,0.328125000,0.332031250,0.335937500,0.339843750,
  0.343750000,0.347656250,0.351562500,0.355468750,0.359375000,0.363281250,0.367187500,0.371093750,
  0.375000000,0.378906250,0.382812500,0.386718750,0.390625000,0.394531250,0.398437500,0.402343750,
  0.406250000,0.410156250,0.414062500,0.417968750,0.421875000,0.425781250,0.429687500,0.433593750,
  0.437500000,0.441406250,0.445312500,0.449218750,0.453125000,0.457031250,0.460937500,0.464843750,
  0.468750000,0.472656250,0.476562500,0.480468750,0.484375000,0.488281250,0.492187500,0.496093750,
  0.500000000,0.503906250,0.507812500,0.511718750,0.515625000,0.519531250,0.523437500,0.527343750,
  0.531250000,0.535156250,0.539062500,0.542968750,0.546875000,0.550781250,0.554687500,0.558593750,
  0.562500000,0.566406250,0.570312500,0.574218750,0.578125000,0.582031250,0.585937500,0.589843750,
  0.593750000,0.597656250,0.601562500,0.605468750,0.609375000,0.613281250,0.617187500,0.621093750,
  0.625000000,0.628906250,0.632812500,0.636718750,0.640625000,0.644531250,0.648437500,0.652343750,
  0.656250000,0.660156250,0.664062500,0.667968750,0.671875000,0.675781250,0.679687500,0.683593750,
  0.687500000,0.691406250,0.695312500,0.699218750,0.703125000,0.707031250,0.710937500,0.714843750,
  0.718750000,0.722656250,0.726562500,0.730468750,0.734375000,0.738281250,0.742187500,0.746093750,
  0.750000000,0.753906250,0.757812500,0.761718750,0.765625000,0.769531250,0.773437500,0.777343750,
  0.781250000,0.785156250,0.789062500,0.792968750,0.796875000,0.800781250,0.804687500,0.808593750,
  0.812500000,0.816406250,0.820312500,0.824218750,0.828125000,0.832031250,0.835937500,0.839843750,
  0.843750000,0.847656250,0.851562500,0.855468750,0.859375000,0.863281250,0.867187500,0.871093750,
  0.875000000,0.878906250,0.882812500,0.886718750,0.890625000,0.894531250,0.898437500,0.902343750,
  0.906250000,0.910156250,0.914062500,0.917968750,0.921875000,0.925781250,0.929687500,0.933593750,
  0.937500000,0.941406250,0.945312500,0.949218750,0.953125000,0.957031250,0.960937500,0.964843750,
  0.968750000,0.972656250,0.976562500,0.980468750,0.984375000,0.988281250,0.992187500,0.996093750,
  1.000000000,1.003906250,1.007812500,1.011718750,1.015625000,1.019531250,1.023437500,1.027343750,
  1.031250000,1.035156250,1.039062500,1.042968750,1.046875000,1.050781250,1.054687500,1.058593750,
  1.062500000,1.066406250,1.070312500,1.074218750,1.078125000,1.082031250,1.085937500,1.089843750,
  1.093750000,1.097656250,1.101562500,1.105468750,1.109375000,1.113281250,1.117187500,1.121093750,
  1.125000000,1.128906250,1.132812500,1.136718750,1.140625000,1.144531250,1.148437500,1.152343750,
  1.156250000,1.160156250,1.164062500,1.167968750,1.171875000,1.175781250,1.179687500,1.183593750,
  1.187500000,1.191406250,1.195312500,1.199218750,1.203125000,1.207031250,1.210937500,1.214843750,
  1.218750000,1.222656250,1.226562500,1.230468750,1.234375000,1.238281250,1.242187500,1.246093750,
  1.250000000,1.253906250,1.257812500,1.261718750,1.265625000,1.269531250,1.273437500,1.277343750,
  1.281250000,1.285156250,1.289062500,1.292968750,1.296875000,1.300781250,1.304687500,1.308593750,
  1.312500000,1.316406250,1.320312500,1.324218750,1.328125000,1.332031250,1.335937500,1.339843750,
  1.343750000,1.347656250,1.351562500,1.355468750,1.359375000,1.363281250,1.367187500,1.371093750,
  1.375000000,1.378906250,1.382812500,1.386718750,1.390625000,1.394531250,1.398437500,1.402343750,
  1.406250000,1.410156250,1.414062500,1.417968750,1.421875000,1.425781250,1.429687500,1.433593750,
  1.437500000,1.441406250,1.445312500,1.449218750,1.453125000,1.457031250,1.460937500,1.464843750,
  1.468750000,1.472656250,1.476562500,1.480468750,1.484375000,1.488281250,1.492187500,1.496093750,
  1.500000000,1.503906250,1.507812500,1.511718750,1.515625000,1.519531250,1.523437500,1.527343750,
  1.531250000,1.535156250,1.539062500,1.542968750,1.546875000,1.550781250,1.554687500,1.558593750,
  1.562500000,1.566406250,1.570312500,1.574218750,1.578125000,1.582031250,1.585937500,1.589843750,
  1.593750000,1.597656250,1.601562500,1.605468750,1.609375000,1.613281250,1.617187500,1.621093750,
  1.625000000,1.628906250,1.632812500,1.636718750,1.640625000,1.644531250,1.648437500,1.652343750,
  1.656250000,1.660156250,1.664062500,1.667968750,1.671875000,1.675781250,1.679687500,1.683593750,
  1.687500000,1.691406250,1.695312500,1.699218750,1.703125000,1.707031250,1.710937500,1.714843750,
  1.718750000,1.722656250,1.726562500,1.730468750,1.734375000,1.738281250,1.742187500,1.746093750,
  1.750000000,1.753906250,1.757812500,1.761718750,1.765625000,1.769531250,1.773437500,1.777343750,
  1.781250000,1.785156250,1.789062500,1.792968750,1.796875000,1.800781250,1.804687500,1.808593750,
  1.812500000,1.816406250,1.820312500,1.824218750,1.828125000,1.832031250,1.835937500,1.839843750,
  1.843750000,1.847656250,1.851562500,1.855468750,1.859375000,1.863281250,1.867187500,1.871093750,
  1.875000000,1.878906250,1.882812500,1.886718750,1.890625000,1.894531250,1.898437500,1.902343750,
  1.906250000,1.910156250,1.914062500,1.917968750,1.921875000,1.925781250,1.929687500,1.933593750,
  1.937500000,1.941406250,1.945312500,1.949218750,1.953125000,1.957031250,1.960937500,1.964843750,
  1.968750000,1.972656250,1.976562500,1.980468750,1.984375000,1.988281250,1.992187500,1.996093750,
  2.000000000,1.996093750,1.992187500,1.988281250,1.984375000,1.980468750,1.976562500,1.972656250,
  1.968750000,1.964843750,1.960937500,1.957031250,1.953125000,1.949218750,1.945312500,1.941406250,
  1.937500000,1.933593750,1.929687500,1.925781250,1.921875000,1.917968750,1.914062500,1.910156250,
  1.906250000,1.902343750,1.898437500,1.894531250,1.890625000,1.886718750,1.882812500,1.878906250,
  1.875000000,1.871093750,1.867187500,1.863281250,1.859375000,1.855468750,1.851562500,1.847656250,
  1.843750000,1.839843750,1.835937500,1.832031250,1.828125000,1.824218750,1.820312500,1.816406250,
  1.812500000,1.808593750,1.804687500,1.800781250,1.796875000,1.792968750,1.789062500,1.785156250,
  1.781250000,1.777343750,1.773437500,1.769531250,1.765625000,1.761718750,1.757812500,1.753906250,
  1.750000000,1.746093750,1.742187500,1.738281250,1.734375000,1.730468750,1.726562500,1.722656250,
  1.718750000,1.714843750,1.710937500,1.707031250,1.703125000,1.699218750,1.695312500,1.691406250,
  1.687500000,1.683593750,1.679687500,1.675781250,1.671875000,1.667968750,1.664062500,1.660156250,
  1.656250000,1.652343750,1.648437500,1.644531250,1.640625000,1.636718750,1.632812500,1.628906250,
  1.625000000,1.621093750,1.617187500,1.613281250,1.609375000,1.605468750,1.601562500,1.597656250,
  1.593750000,1.589843750,1.585937500,1.582031250,1.578125000,1.574218750,1.570312500,1.566406250,
  1.562500000,1.558593750,1.554687500,1.550781250,1.546875000,1.542968750,1.539062500,1.535156250,
  1.531250000,1.527343750,1.523437500,1.519531250,1.515625000,1.511718750,1.507812500,1.503906250,
  1.500000000,1.496093750,1.492187500,1.488281250,1.484375000,1.480468750,1.476562500,1.472656250,
  1.468750000,1.464843750,1.460937500,1.457031250,1.453125000,1.449218750,1.445312500,1.441406250,
  1.437500000,1.433593750,1.429687500,1.425781250,1.421875000,1.417968750,1.414062500,1.410156250,
  1.406250000,1.402343750,1.398437500,1.394531250,1.390625000,1.386718750,1.382812500,1.378906250,
  1.375000000,1.371093750,1.367187500,1.363281250,1.359375000,1.355468750,1.351562500,1.347656250,
  1.343750000,1.339843750,1.335937500,1.332031250,1.328125000,1.324218750,1.320312500,1.316406250,
  1.312500000,1.308593750,1.304687500,1.300781250,1.296875000,1.292968750,1.289062500,1.285156250,
  1.281250000,1.277343750,1.273437500,1.269531250,1.265625000,1.261718750,1.257812500,1.253906250,
  1.250000000,1.246093750,1.242187500,1.238281250,1.234375000,1.230468750,1.226562500,1.222656250,
  1.218750000,1.214843750,1.210937500,1.207031250,1.203125000,1.199218750,1.195312500,1.191406250,
  1.187500000,1.183593750,1.179687500,1.175781250,1.171875000,1.167968750,1.164062500,1.160156250,
  1.156250000,1.152343750,1.148437500,1.144531250,1.140625000,1.136718750,1.132812500,1.128906250,
  1.125000000,1.121093750,1.117187500,1.113281250,1.109375000,1.105468750,1.101562500,1.097656250,
  1.093750000,1.089843750,1.085937500,1.082031250,1.078125000,1.074218750,1.070312500,1.066406250,
  1.062500000,1.058593750,1.054687500,1.050781250,1.046875000,1.042968750,1.039062500,1.035156250,
  1.031250000,1.027343750,1.023437500,1.019531250,1.015625000,1.011718750,1.007812500,1.003906250,
  1.000000000,0.996093750,0.992187500,0.988281250,0.984375000,0.980468750,0.976562500,0.972656250,
  0.968750000,0.964843750,0.960937500,0.957031250,0.953125000,0.949218750,0.945312500,0.941406250,
  0.937500000,0.933593750,0.929687500,0.925781250,0.921875000,0.917968750,0.914062500,0.910156250,
  0.906250000,0.902343750,0.898437500,0.894531250,0.890625000,0.886718750,0.882812500,0.878906250,
  0.875000000,0.871093750,0.867187500,0.863281250,0.859375000,0.855468750,0.851562500,0.847656250,
  0.843750000,0.839843750,0.835937500,0.832031250,0.828125000,0.824218750,0.820312500,0.816406250,
  0.812500000,0.808593750,0.804687500,0.800781250,0.796875000,0.792968750,0.789062500,0.785156250,
  0.781250000,0.777343750,0.773437500,0.769531250,0.765625000,0.761718750,0.757812500,0.753906250,
  0.750000000,0.746093750,0.742187500,0.738281250,0.734375000,0.730468750,0.726562500,0.722656250,
  0.718750000,0.714843750,0.710937500,0.707031250,0.703125000,0.699218750,0.695312500,0.691406250,
  0.687500000,0.683593750,0.679687500,0.675781250,0.671875000,0.667968750,0.664062500,0.660156250,
  0.656250000,0.652343750,0.648437500,0.644531250,0.640625000,0.636718750,0.632812500,0.628906250,
  0.625000000,0.621093750,0.617187500,0.613281250,0.609375000,0.605468750,0.601562500,0.597656250,
  0.593750000,0.589843750,0.585937500,0.582031250,0.578125000,0.574218750,0.570312500,0.566406250,
  0.562500000,0.558593750,0.554687500,0.550781250,0.546875000,0.542968750,0.539062500,0.535156250,
  0.531250000,0.527343750,0.523437500,0.519531250,0.515625000,0.511718750,0.507812500,0.503906250,
  0.500000000,0.496093750,0.492187500,0.488281250,0.484375000,0.480468750,0.476562500,0.472656250,
  0.468750000,0.464843750,0.460937500,0.457031250,0.453125000,0.449218750,0.445312500,0.441406250,
  0.437500000,0.433593750,0.429687500,0.425781250,0.421875000,0.417968750,0.414062500,0.410156250,
  0.406250000,0.402343750,0.398437500,0.394531250,0.390625000,0.386718750,0.382812500,0.378906250,
  0.375000000,0.371093750,0.367187500,0.363281250,0.359375000,0.355468750,0.351562500,0.347656250,
  0.343750000,0.339843750,0.335937500,0.332031250,0.328125000,0.324218750,0.320312500,0.316406250,
  0.312500000,0.308593750,0.304687500,0.300781250,0.296875000,0.292968750,0.289062500,0.285156250,
  0.281250000,0.277343750,0.273437500,0.269531250,0.265625000,0.261718750,0.257812500,0.253906250,
  0.250000000,0.246093750,0.242187500,0.238281250,0.234375000,0.230468750,0.226562500,0.222656250,
  0.218750000,0.214843750,0.210937500,0.207031250,0.203125000,0.199218750,0.195312500,0.191406250,
  0.187500000,0.183593750,0.179687500,0.175781250,0.171875000,0.167968750,0.164062500,0.160156250,
  0.156250000,0.152343750,0.148437500,0.144531250,0.140625000,0.136718750,0.132812500,0.128906250,
  0.125000000,0.121093750,0.117187500,0.113281250,0.109375000,0.105468750,0.101562500,0.097656250,
  0.093750000,0.089843750,0.085937500,0.082031250,0.078125000,0.074218750,0.070312500,0.066406250,
  0.062500000,0.058593750,0.054687500,0.050781250,0.046875000,0.042968750,0.039062500,0.035156250,
  0.031250000,0.027343750,0.023437500,0.019531250,0.015625000,0.011718750,0.007812500,0.003906250
};

#endif /* #ifdef BARTLETT_WINDOW */


static void four1(float * data, unsigned long nn, int isign)
{
  uint16_t n,mmax,m,j,istep,i;
  float wtemp,wr,wpr,wpi,wi,theta;
  float tempr,tempi;

  n=nn << 1;
  j=1;
  for (i=1;i<n;i+=2)
  {
    if (j > i)
    {
      // swap first
      tempr = data[j];
      data[j]= data[i];
      data[i] = tempr;
      // swap second
      tempr = data[j+1];
      data[j+1]=data[i+1];
      data[i+1]=tempr;
    }
    m=n >> 1;
    while (m >= 2 && j > m)
    {
      j -= m;
      m >>= 1;
    }
    j += m;
  }
  mmax=2;
  while (n > mmax)
  {
    istep=mmax << 1;
    theta=isign*(6.28318530717959/mmax);
    wtemp=sinf(0.5*theta);
    wpr = -2.0*wtemp*wtemp;
    wpi=sinf(theta);
    wr=1.0;
    wi=0.0;
    for (m=1;m<mmax;m+=2) {
      for (i=m;i<=n;i+=istep) {
        j=i+mmax;
        tempr=wr*data[j]-wi*data[j+1];
        tempi=wr*data[j+1]+wi*data[j];
        data[j]=data[i]-tempr;
        data[j+1]=data[i+1]-tempi;
        data[i] += tempr;
        data[i+1] += tempi;
      }
      wr=(wtemp=wr)*wpr-wi*wpi+wr;
      wi=wi*wpr+wtemp*wpi+wi;
    }
    mmax=istep;
  }
}

void realft(float *data, uint32_t n, int32_t isign)
{
  uint16_t i,i1,i2,i3,i4,np3;
  float c1=0.5,c2,h1r,h1i,h2r,h2i;
  float wr,wi,wpr,wpi,wtemp,theta;

  theta=3.141592653589793/(float) (n>>1);
  if (isign == 1)
  {
    c2 = -0.5;
    four1(data,n>>1,1);
  }
  else
  {
    c2=0.5;
    theta = -theta;
  }
  wtemp=sinf(0.5*theta);
  wpr = -2.0*wtemp*wtemp;
  wpi=sinf(theta);
  wr=1.0+wpr;
  wi=wpi;
  np3=n+3;
  for (i=2;i<=(n>>2);i++)
  {
    i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
    h1r=c1*(data[i1]+data[i3]);
    h1i=c1*(data[i2]-data[i4]);
    h2r = -c2*(data[i2]+data[i4]);
    h2i=c2*(data[i1]-data[i3]);
    data[i1]=h1r+wr*h2r-wi*h2i;
    data[i2]=h1i+wr*h2i+wi*h2r;
    data[i3]=h1r-wr*h2r+wi*h2i;
    data[i4] = -h1i+wr*h2i+wi*h2r;
    wr=(wtemp=wr)*wpr-wi*wpi+wr;
    wi=wi*wpr+wtemp*wpi+wi;
  }
  if (isign == 1)
  {
    h1r=data[1];
    data[1] = h1r+data[2];
    data[2] = h1r-data[2];
  }
  else
  {
    h1r=data[1];
    data[1]=c1*(h1r+data[2]);
    data[2]=c1*(h1r-data[2]);
    four1(data,n>>1,-1);
  }
}

float window(uint16_t *x_t, uint32_t m0, float *f, uint32_t len)
{
  uint32_t m;
  float rval;

  // m0 is the index of the latest point put in the circular ISR buffer
  // we increase it by one, as this point is the first in line to be
  // replaced by fresh datum.
  // We do that two times in order to prevent incidental write while
  // we are unrolling data

  // copy the data from ISR buffer to FFT buffer so it
  // doesn't change during computations:
  //   - in doing so we unroll the circular ISR buffer
  //   - this is important because of the windowing function that
  //   we apply later
  // compute the mean value in this pass
  rval = 0.0;
  for (m=1; m<len; m++)
  {
    f[m]  = (float) x_t[ (m0 + m) % len];
    rval += f[m];
  }
  rval /= ((float) len - 1.0f);

  // make sure that there is some signal if we are to do the FFT
  if (rval*rval < 1.0f)
  {
    rval = 0.0;
    return rval;
  }

  float rcp_rval=1.0f/rval, tmp;

  // unit-gain window:
  //  skip first term because its weight is zero for all window functions except rectangular
  f[0] = 0.0f;
  for (m=1; m<len; m++)
  {
    tmp = f[m] * rcp_rval - 1.0f;
#if defined(SINE_WINDOW)
    f[m] = window_sine_unit_gain_1024[m] * tmp;
#elif defined(BARTLETT_WINDOW)
    f[m] = window_bartlett_unit_gain_1024[m] * tmp;
#else
    f[m] = tmp;
#endif
  }

  return rval;
}


/**
  * range of modulation amplitudes:
  *       u(nity) -> 0:1
  *       d(eci)  -> 0:(0.1)
  *       c(enti) -> 0:(0.01)
  *       m(ili)  -> 0:(0.001)
  *  which is scaled to 0:255 absolute for printout (one byte per amplitude)
  *  clipped on the top, and disregarded on the bottom
  */
static const float
    fft_coef_min[] = {0.5f/255.0f,  0.05f/255.0f, 0.005f/255.0f,  0.0005f/255.0f};

static const float
    fft_coef_max[] = {1.0f,         0.1f,         0.01f,          0.001f};

#define FFT_COEF_RAW_SCALE ((float) 2.0f/(1024.0f))
#define FFT_COEF_SQ_RAW_SCALE (FFT_COEF_RAW_SCALE * FFT_COEF_RAW_SCALE)


uint32_t convert_fft_to_modamps(float *f, uint32_t len_f, uint32_t max_amp)
{
  uint16_t i, ievn, iodd;
  float coef_2;
  float min_fft_coef  = fft_coef_min[max_amp] / FFT_COEF_RAW_SCALE;
  float fft_scaling   = 65025.0f * FFT_COEF_SQ_RAW_SCALE / (fft_coef_max[max_amp] * fft_coef_max[max_amp]);
  float min_fft_coef2 = min_fft_coef * min_fft_coef;

  uint32_t iend = (len_f>>1) - 1;

  while (1)
  {
    if (f[(iend<<1)]   >  min_fft_coef)
      break;
    if (f[(iend<<1)]   < -min_fft_coef)
      break;
    if (f[(iend<<1)+1] >  min_fft_coef)
      break;
    if (f[(iend<<1)+1] < -min_fft_coef)
      break;

    if (!iend)
      return (iend);

    iend--;
  }

  for (i=1; i<=iend; i++)
  {
    ievn = i<<1;
    iodd = ievn + 1;

    coef_2 = (f[ievn]*f[ievn] + f[iodd]*f[iodd]) * fft_scaling;
    if (coef_2 > 65025.0f)
    {
      f[ievn] = 255.0f;
      continue;
    }
    if (coef_2 < 0.25f)
    {
      f[ievn] = 0.0;
      continue;
    }

    f[ievn]  = sqrtf(coef_2);
  }

  return (iend);
}

//-------------------------------------------------------
//
// FFT - END
//
//-------------------------------------------------------
