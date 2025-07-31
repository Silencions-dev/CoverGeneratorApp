#include "HPCover.hpp"

/*!
 * \brief Initialization of the class HPCover
 *
 * Fill the stored arrays with zeros - dynamic arrays in the
 * initialization list and static arrays using for loops.
 */
HPCover::HPCover() : inner_dimensions(DimNum, 0),
                     outer_dimensions(DimNum, 0),
                     approx_inner_dim(DimNum, 0)
{
    for(uint8_t i = 0; i < DimNum; ++i)
    {
        acc_in_dimensions[i] = 0;
        acc_out_dimensions[i] = 0;
    }

    for(uint8_t i = 0; i < DevSides; ++i)
        inner_offsets[i] = 0;
}
/*!
 * \brief Generate the cover
 *
 * Overwrite the stored inputs from the user with the new ones.
 * Sort the dimensions of the base parts of the cover (from the
 * configuration file) in order to make sure that they can be
 * correctly found later. Then count all inner dimensions and
 * check if they exceed the limits. If not, iterate through the
 * available parts and find the ones that match to the calculated
 * inner dimensions. If parts have been found, determine the exact
 * inner and outer dimensions of the cover. If the cover has been
 * correctly generated, determine the collisions with the nearby
 * obstacles (if present).
 *
 * \param[in] dimensions - input data from the user
 * \return True if generated, false if not
 */
bool HPCover::generateCover(std::vector<std::vector<int>> dimensions)
{
    // Nadpisanie macierzy danych wejściowych (możliwość aktualizowania wyników)
    dim_matrix = dimensions;

    // Sortowanie wczytanych dostępnych wymiarów (dla pewności)
    for(uint8_t i = 0; i < DimNum; ++i)
        std::sort(base_dimensions[i].begin(), base_dimensions[i].end());

    // Obliczenia wszystkich POTRZEBNYCH wymiarów wewnętrznych
    bool length_correct = countInnerLength();
    bool width_correct = countInnerWidth();
    bool height_correct = countInnerHeight();

    if(!length_correct)
        error_handler.raise(GenErrors::TOO_LONG);
    else if(!width_correct)
        error_handler.raise(GenErrors::TOO_WIDE);
    else if(!height_correct)
        error_handler.raise(GenErrors::TOO_HIGH);

    if(!(length_correct & width_correct & height_correct))
        return false;
    // Przeszukanie bilbioteki części
    if(!searchPartsLibrary())
        return false;
    // Obliczenie wymiarów zewnętrznych wygenerowanej obudowy
    countOuterDimensions();

    // Sprawdzenie kolizji z ewentualnymi przeszkodami
    bool no_left_collision = checkForLengthCollision(dim_matrix[1][0]);
    bool no_right_collision = checkForLengthCollision(dim_matrix[1][1]);
    bool no_back_collision = checkForWidthCollision(dim_matrix[1][2]);

    if(!no_left_collision)
        this->error_handler.raise(GenErrors::LEFT_COLLISION);
    else if(!no_right_collision)
        this->error_handler.raise(GenErrors::RIGHT_COLLISION);
    else if(!no_back_collision)
        this->error_handler.raise(GenErrors::BACK_COLLISION);

    if(!(no_left_collision & no_right_collision & no_back_collision))
        return false;

    return true;
}
/*!
 * \brief Calculate the required width
 *
 * First, determine the maximum width of the cover based on the
 * available parts. Then, calculate the required width with addition
 * of the left and right inner spaces between cover and heat pump (from the user or
 * default if not given). Check if it exceeds the limit. If so, return
 * false (required cover is too wide).
 *
 * \return True when value is lower than the max threshold and false otherwise
 */
bool HPCover::countInnerLength()
{
    int max_length = base_dimensions[0].back() + acc_in_dimensions[0];
    // Dodanie domyślnej wartości odstępu od boków wewnątrz
    // jeżeli użytkownik nie podał tej zmiennej (wpisane 0).
    dim_matrix[2][0] = (dim_matrix[2][0] == -1) ? inner_offsets[1] : dim_matrix[2][0];

    approx_inner_dim[0] = dim_matrix[0][0] + 2 * dim_matrix[2][0];

    if(approx_inner_dim[0] > max_length)
        return false;

    return true;
}
/*!
 * \brief Calculate the required depth
 *
 * First, determine the maximum depth of the cover based on the
 * available parts. Then, calculate the required depth with addition
 * of the front and back inner spaces between cover and heat pump (from the user or
 * default if not given). Check if it exceeds the limit. If so, return
 * false (required cover is too deep).
 *
 * \return True when value is lower than the max threshold and false otherwise
 */
bool HPCover::countInnerWidth()
{
    int max_width = base_dimensions[1].back() + acc_in_dimensions[1];
    // Obliczenie wymiaru wewnętrznego i ustalenie typu obudowy
    // zależnie od odległości z tyłu (do eleweacji).
    dim_matrix[2][1] = (dim_matrix[2][1] == -1) ? inner_offsets[0] : dim_matrix[2][1];
    dim_matrix[2][2] = (dim_matrix[2][2] == -1) ? inner_offsets[2] : dim_matrix[2][2];

    approx_inner_dim[1] = dim_matrix[0][1] + dim_matrix[2][1] + dim_matrix[2][2];

    if(approx_inner_dim[1] > max_width)
        return false;

    return true;
}
/*!
 * \brief Calculate the required height
 *
 * First, determine the maximum height of the cover based on the
 * available parts and maximum number of wall modules. Then, calculate
 * the required height with addition of the top inner space
 * between cover and heat pump (from the user or default if not given).
 * Check if it exceeds the limit. If so, return false (required cover is
 * too high).
 *
 * \return True when value is lower than the max threshold and false otherwise
 */
bool HPCover::countInnerHeight()
{
    int max_height = base_dimensions[2][0] + base_dimensions[2][2];
    max_height += base_dimensions[2][1] * MaxWallModules;

    dim_matrix[2][3] = (dim_matrix[2][3] == -1) ? inner_offsets[3] : dim_matrix[2][3];

    approx_inner_dim[2] = dim_matrix[0][2] + dim_matrix[2][3];

    if(approx_inner_dim[2] > max_height)
        return false;

    return true;
}
/*!
 * \brief Calculate the precise outer dimensions of the generated cover
 *
 * Outer dimension is the addition of the inner dimension calculated
 * from the rounded parts dimensions and the precise values (corrections)
 * from the CAD program.
 */
void HPCover::countOuterDimensions()
{
    for(uint8_t i = 0; i < DimNum; ++i)
        outer_dimensions[i] = inner_dimensions[i] + acc_out_dimensions[i];
}
/*!
 * \brief Check if there is a collision with the object on the left or right
 *
 * If distance to the obstacle is not given (-1), return true (no collision).
 * Compare the half of the outer width of the cover with the distance to
 * the obstacle from the center of the heat pump (assuming that centers of the
 * cover and device will be aligned vertically.)
 *
 * \param[in] constraint - distance to the obstacle typed by the user
 * \return True for collision and false otherwise
 */
bool HPCover::checkForLengthCollision(int constraint)
{
    if(constraint == -1)
        return true;

    int cover_outer_dim = outer_dimensions[0];
    int dimension = dim_matrix[0][0];
    int half_cover_dim = ceil(cover_outer_dim / 2);
    int dist_to_obstacle = floor(dimension / 2) + constraint;

    if(dist_to_obstacle <= half_cover_dim)
        return false;

    return true;
}
/*!
 * \brief Check if there is a collision with the object behind the device
 *
 * If distance to the obstacle is not given (-1), return true (no collision).
 * Compare the half of the outer depth of the cover with the distance to
 * the obstacle from the center of the heat pump. Device and cover do not have
 * to be algined, the front and back inner spaces are taken into account.
 *
 * \param[in] constraint - distance to the obstacle typed by the user
 * \return True for collision and false otherwise
 */
bool HPCover::checkForWidthCollision(int constraint)
{
    if(constraint == -1)
        return true;

    int cover_outer_dim = outer_dimensions[1];
    int dimension = dim_matrix[0][1];
    int dist_to_obstacle = floor(dimension / 2) + constraint;

    int offset = ceil(inner_dimensions[1] / 2) - ceil(dimension / 2) - dim_matrix[2][2];
    int half_cover_dim = ceil(cover_outer_dim / 2) - offset;

    if(dist_to_obstacle <= half_cover_dim)
        return false;

    return true;
}
/*!
 * \brief Return currently stored user data
 *
 * \return Reference to the dynamic array with user input data
 */
std::vector<std::vector<int>> & HPCover::returnFinalDimensions()
{
    return dim_matrix;
}
/*!
 * \brief Check if the required dimensions can be provided by the available parts
 *
 * Find the available width and depth of the cover for the required
 * inner dimensions (from the user). If returend value is -1, then it
 * means that the part has not been found and the appropriate error is
 * raised. If both width and depth are available, calculate how many wall
 * modules of the standard cover is necessary.
 *
 * \return True if standard cover can be made and false otherwise (too large)
 */
bool HPCover::searchPartsLibrary()
{
    // Znajdź pasującą długość i szerokość i zaktualizuj wymiary wewnętrzne
    int available_length = static_cast<int>(pickRightDimension(DIM_ID::LENGTH));
    int available_width = static_cast<int>(pickRightDimension(DIM_ID::WIDTH));
    // Nie da się wygenerować obudowy standardowej
    if(available_length == -1)
    {
        error_handler.raise(GenErrors::NO_PART_LENGTH);
        return false;
    }
    if(available_width == -1)
    {
        error_handler.raise(GenErrors::NO_PART_WIDTH);
        return false;
    }
    // Znajdź ilość potrzebnych modułów i zaktualizuj wysokość wewnętrzną
    modules = pickRightModulesQuantity();

    return true;
}
/*!
 * \brief Find the base dimension available for the approximated one
 *
 * Iterate over the list of available dimensions (given by the input
 * argument 'dim_idx') and find the first value that is greater than
 * or equal to dimension calculated from the input data provided by
 * the user. Available dimensions are sorted, thus there is no need
 * to iterate over the whole list each time. If the dim
 *
 * \param[in] dim_idx - required dimension (enumeration)
 * \return Found dimension from the list of available ones (parts of the cover)
 */
float HPCover::pickRightDimension(DIM_ID dim_idx)
{
    float found_dimension = -1;

    // Znajdź pierwszy napotkany pasujący wymiar - długość lub szerokość
    for(uint8_t j = 0; j < base_dimensions[dim_idx].size(); ++j)
    {
        // Dokładny dostępny wymiar
        float acc_available_dim = base_dimensions[dim_idx][j] + acc_in_dimensions[dim_idx];
        // Jeżeli dostępny wymiar jest większy lub równy niż wymagany to:
        if(acc_available_dim >= approx_inner_dim[dim_idx])
        {
            found_dimension = base_dimensions[dim_idx][j];
            break;
        }
    }
    // Zapisanie oficjalnego wyniku
    inner_dimensions[dim_idx] = found_dimension + acc_in_dimensions[dim_idx];

    return found_dimension;
}
/*!
 * \brief Choose the appropriate number of the wall modules to the required height
 *
 * Calculate the minimum height that is required by the user
 * and then increment the number of modules until the raising
 * height is greater than or equal to the minimum height.
 * At the end update the precise inner height of the cover.
 *
 * \return Total number of the wall modules (for both walls)
 */
uint8_t HPCover::pickRightModulesQuantity()
{
    // Minimalna wysokośc obudowy (moduł dolny i górny z poprawką dokładności)
    uint16_t sum_height = base_dimensions[DIM_ID::HEIGHT][0] +
                          base_dimensions[DIM_ID::HEIGHT][2] +
                          acc_in_dimensions[DIM_ID::HEIGHT];

    uint8_t std_modules_pcs = 0;

    while(true)
    {
        if(sum_height >= approx_inner_dim[DIM_ID::HEIGHT])
            break;

        sum_height += base_dimensions[DIM_ID::HEIGHT][1];
        ++std_modules_pcs;
    }
    // Zapisanie oficjalnego wyniku
    inner_dimensions[DIM_ID::HEIGHT] = sum_height;

    return std_modules_pcs * 2;
}
/*!
 * \brief Display the loaded configuration parameters (debug only)
 *
 * Print all the parameters loaded from the configuration file
 * on the standard output, usually terminal. It is used to check
 * if the configuration file has been correctly parsed.
 */
void HPCover::printResults()
{
    std::cout << "\nBase dimensions: \n";
    for(uint8_t i = 0; i < DimNum; ++i)
    {
        std::cout << "\t";
        for(uint8_t j = 0; j < base_dimensions[i].size(); ++j)
            std::cout << base_dimensions[i][j] << " ";

        std::cout << "\n";
    }

    std::cout << "\nOuter dimensions add-ons: \n\t";
    for(uint8_t i = 0; i < DimNum; ++i)
        std::cout << acc_out_dimensions[i] << " ";

    std::cout << "\n\nInner dimensions add-ons: \n\t";
    for(uint8_t i = 0; i < DimNum; ++i)
        std::cout << acc_in_dimensions[i] << " ";

    std::cout << "\n\nMinimal spaces: \n\t";
    for(uint8_t i = 0; i < DevSides; ++i)
        std::cout << inner_offsets[i] << " ";

    std::cout << "\n\nWall offset: \n\t" << wall_offset << "\n";
}
/*!
 * \brief Read parameters of the generator from the configuration file
 *
 * Open the configuration file and start reading line by line if
 * opened without errors. Omit all lines, which start with the '#'
 * sign (considered as comments) or are simply empty. Turn the line
 * into a string stream and pass to the parser, which interprets
 * the string and distribute the values to the correct buffers.
 *
 * \return True if correctly loaded and false otherwise
 */
bool HPCover::readParameters()
{
    QFile param_file(GenConfigPath);

    if(param_file.open(QIODevice::ReadOnly))
    {
        QTextStream text_str(&param_file);

        while(!text_str.atEnd())
        {
            QString qt_line = text_str.readLine();
            // Conversion to STD string
            std::string line = qt_line.toStdString();
            // Pominięcie komentarzy
            if(line[0] == '#' || line.empty())
                continue;

            std::stringstream line_stream(line);

            if(!readLine(line_stream))
                return false;
        }
    }
    else
    {
        qDebug() << "\nUnable to open HP cover parameters file. Exit...\n";
        return false;
    }

    param_file.close();

    return true;
}
/*!
 * \brief Read and analyze the line of the file
 *
 * Line should start with the name of the variable and the semicolon
 * and so it is verified at the beginning. If the line starts in this
 * way, then name of the variable is compared with the ones defined in
 * the dedicated structure. If name matches one of the strings, interpret
 * the data correctly.
 *
 * \param[in] stream - line in the form of the string stream (easy to parse)
 * \return True if succeed and false otherwise
 */
bool HPCover::readLine(std::stringstream &stream)
{
    std::string var_name;
    char sym = '\0';

    stream >> var_name >> sym;

    if(sym != ':')
    {
        qDebug() << "\nError during reading the file. Lack of ':' symbol. Abort.\n";
        return false;
    }

    std::string value;

    if(var_name == params.BASE_LENGTH)
    {
        while(stream >> value)
            base_dimensions[0].push_back(std::stof(value));
    }
    else if(var_name == params.BASE_WIDTH)
    {
        while(stream >> value)
            base_dimensions[1].push_back(std::stof(value));
    }
    else if(var_name == params.BASE_HEIGHT)
    {
        while(stream >> value)
            base_dimensions[2].push_back(std::stof(value));
    }
    else if(var_name == params.OUT_LENGTH)
    {
        stream >> value;
        acc_out_dimensions[0] = std::stof(value);
    }
    else if(var_name == params.OUT_WIDTH)
    {
        stream >> value;
        acc_out_dimensions[1] = std::stof(value);
    }
    else if(var_name == params.OUT_HEIGHT)
    {
        stream >> value;
        acc_out_dimensions[2] = std::stof(value);
    }
    else if(var_name == params.ACC_LENGTH)
    {
        stream >> value;
        acc_in_dimensions[0] = std::stof(value);
    }
    else if(var_name == params.ACC_WIDTH)
    {
        stream >> value;
        acc_in_dimensions[1] = std::stof(value);
    }
    else if(var_name == params.ACC_HEIGHT)
    {
        stream >> value;
        acc_in_dimensions[2] = std::stof(value);
    }
    else if(var_name == params.FRONT_SPACE)
    {
        stream >> value;
        inner_offsets[0] = std::stof(value);
    }
    else if(var_name == params.SIDE_SPACE)
    {
        stream >> value;
        inner_offsets[1] = std::stof(value);
    }

    else if(var_name == params.BACK_SPACE)
    {
        stream >> value;
        inner_offsets[2] = std::stof(value);
    }

    else if(var_name == params.TOP_SPACE)
    {
        stream >> value;
        inner_offsets[3] = std::stof(value);
    }
    else if(var_name == params.WALL_SPACE)
    {
        stream >> value;
        wall_offset = std::stof(value);
    }

    return true;
}
