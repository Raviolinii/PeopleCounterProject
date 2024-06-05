#See https://aka.ms/customizecontainer to learn how to customize your debug container and how Visual Studio uses this Dockerfile to build your images for faster debugging.

FROM mcr.microsoft.com/dotnet/aspnet:7.0 AS base
WORKDIR /app
EXPOSE 80
EXPOSE 443

FROM mcr.microsoft.com/dotnet/sdk:7.0 AS build
WORKDIR /src
COPY ["CounterApi.csproj", "CounterApi/"]
RUN dotnet restore "CounterApi/CounterApi.csproj"

WORKDIR "/src/CounterApi"
COPY . .
RUN dotnet build "CounterApi.csproj" -c Release -o /app/build

FROM build AS publish
RUN dotnet publish "CounterApi.csproj" -c Release -o /app/publish

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .

ENV ASPNETCORE_ENVIRONMENT=Docker

ENTRYPOINT ["dotnet", "CounterApi.dll"]