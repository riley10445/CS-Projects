#include <stdio.h>
#include <string.h>

int main()
{
    char destination[100];
    char city1[100], city2[100];
    int on = 1;

    strcpy(destination, "tuscaloosa");

    while (on)
    {
        printf("From tuscaloosa: new-orleans, nashville, milton, or atlanta? ");
        scanf("%s", destination);
        if (strcmp(destination, "new-orleans") == 0)
        {
            printf("From new-orleans: lafayette, or lake-charles? ");
            scanf("%s", destination);
            if (strcmp(destination, "lake-charles") == 0)
            {
                printf("From lake-charles: shreveport, or alexandria? ");
                scanf("%s", destination);
                on = 0;
            }
            else
            {
                on = 0;
            }
        }
        else if (strcmp(destination, "nashville") == 0)
        {
            printf("From nashville: memphis, or murfreesboro? ");
            scanf("%s", destination);
            if (strcmp(destination, "murfreesboro") == 0)
            {
                printf("From murfreesboro: chattanooga, or knoxville? ");
                scanf("%s", destination);
                if (strcmp(destination, "chattanooga") == 0)
                {
                    printf("From chattanooga: gatlinburg, or asheville? ");
                    scanf("%s", destination);
                    on = 0;
                }
                else
                {
                    on = 0;
                }
            }
            else
            {
                on = 0;
            }
        }
        else if (strcmp(destination, "milton") == 0)
        {
            printf("From milton: pensacola, destin, or panama-city? ");
            scanf("%s", destination);
            if (strcmp(destination, "pensacola") == 0)
            {
                printf("From pensacola: tallahasee, or st-augustine? ");
                scanf("%s", destination);
                if (strcmp(destination, "tallahassee") == 0)
                {
                    printf("From tallahassee: gainesville, or ocala? ");
                    scanf("%s", destination);
                    on = 0;
                }
                else
                {
                    on = 0;
                }
            }
            if (strcmp(destination, "destin") == 0)
            {
                printf("From destin: daytona, or kissimmee? ");
                scanf("%s", destination);
                on = 0;
            }
            if (strcmp(destination, "panama-city") == 0)
            {
                printf("From panama-city: tamps, or jacksonville? ");
                scanf("%s", destination);
                if (strcmp(destination, "jacksonville") == 0)
                {
                    printf("From jacksonville: clearwater, orlando, or st-petersburg? ");
                    scanf("%s", destination);
                    if (strcmp(destination, "clearwater") == 0)
                    {
                        printf("From clearwater: ft-meyers, or miami? ");
                        scanf("%s", destination);
                        on = 0;
                    }
                    else if (strcmp(destination, "orlando") == 0)
                    {
                        printf("From orlando: west-palm, or ft-lauderdale? ");
                        scanf("%s", destination);
                        on = 0;
                    }
                    else
                    {
                        on = 0;
                    }
                }
                else
                {
                    on = 0;
                }
            }
        }
        else
        {
            printf("From atlanta: athens, or columbus? ");
            scanf("%s", destination);
            if (strcmp(destination, "athens") == 0)
            {
                printf("From athens: augusta, or macon? ");
                scanf("%s", destination);
                if (strcmp(destination, "augusta") == 0)
                {
                    printf("From augusta: savannah, or tybee-island? ");
                    scanf("%s", destination);
                    on = 0;
                }
                else
                {
                    on = 0;
                }
            }
            if (strcmp(destination, "columbus") == 0)
            {
                printf("From columbus: warner-robbins, or albany? ");
                scanf("%s", destination);
                if (strcmp(destination, "albany") == 0)
                {
                    printf("From albany: fernandina, or brunswick? ");
                    scanf("%s", destination);
                    on = 0;
                }
                else
                {
                    on = 0;
                }
            }
        }
    }
    printf("You ended your trip in %s!\n", destination);

    return 0;
}
