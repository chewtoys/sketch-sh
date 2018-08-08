Modules.require("./App.css");
open Utils;
open Route;

type state = route;

type action =
  | ChangeView(route);

let component = ReasonReact.reducerComponent("Rtop");

let make = _children => {
  ...component,
  initialState: () =>
    ReasonReact.Router.dangerouslyGetInitialUrl()->Route.urlToRoute,
  reducer: (action, _state) =>
    switch (action) {
    | ChangeView(view) => ReasonReact.Update(view)
    },
  didMount: ({send, onUnmount}) => {
    let watcherID =
      ReasonReact.Router.watchUrl(url =>
        Route.urlToRoute(url)->ChangeView->send
      );
    onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },
  render: ({state}) =>
    <ReasonApollo.Provider client=GqlClient.instance>
      <AuthStatus.Provider>
        (
          switch (state) {
          | Home => <Home />
          | Note(noteInfo) => <Note noteInfo />
          | NoteNew => <Note_New />
          | AuthCallback(token) => <Auth.AuthCallback token />
          | AuthLogout => <Auth.AuthLogout />
          | AuthGithub => <Auth.AuthGithub />
          | AuthFailure => "auth failure"->str
          | EditorDevelopment =>
            Utils.env == "production" ? <NotFound /> : <Editor_Note_Loader />
          | NotFound => <NotFound />
          }
        )
      </AuthStatus.Provider>
    </ReasonApollo.Provider>,
};

let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));
